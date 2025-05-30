#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int sockfd;

void* recvMessage(void* arg){
    char mess[1024];
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    while(1){
        ssize_t n = recv(sockfd, mess, sizeof(mess) - 1, 0);
        if(n < 1){
            perror("recv");
            shutdown(sockfd, SHUT_RDWR);
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        mess[n] = '\0';
        printf("\nServer to Client: %s\n", mess);
        pthread_testcancel();
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]){
    pthread_t tid;
    struct sockaddr_in servaddr;

    if(argc < 3){
        fprintf(stderr, "Usage: %s <server_ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    int res = inet_pton(AF_INET, argv[1], &servaddr.sin_addr);
    if(res <= 0){
        if(res == 0)
            fprintf(stderr, "Invalid IP address format\n");
        else
            perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if(connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0){
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if(pthread_create(&tid, NULL, recvMessage, NULL) < 0){
        perror("pthread_create");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char message[1024];
    while(1){
        if(fgets(message, sizeof(message), stdin) != NULL){
            size_t len = strlen(message);
            if(len > 0 && message[len - 1] == '\n'){
                message[len - 1] = '\0';
            }
        }
        if(strcmp(message, "q") == 0){
            printf("Exit...\n");
            if(pthread_cancel(tid) != 0){
                perror("pthread_cancel");
                exit(EXIT_FAILURE);
            }
            break;
        }
        if(send(sockfd, message, strlen(message), 0) < 0){
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    if(pthread_join(tid, NULL) != 0){
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }

    shutdown(sockfd, SHUT_RDWR);
    close(sockfd);
    exit(EXIT_SUCCESS);
}
