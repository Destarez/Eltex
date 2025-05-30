#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int sockfd;

void* recvMessage(void* arg) {
    char mess[1024];
    while (1) {
        int n = read(sockfd, mess, sizeof(mess));
        if (n <= 0) {
            perror("recv");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("\nServer to Client: %s\n", mess);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t tid;
    struct sockaddr_in servaddr;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 1) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (pthread_create(&tid, NULL, recvMessage, NULL) != 0) {
        perror("pthread_create");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    char message[1024];
    while (1) {
        if (fgets(message, sizeof(message), stdin) != NULL) {
            if (message[strlen(message) - 1] == '\n') {
                message[strlen(message) - 1] = '\0';
            }
        }
        if (!strcmp(message, "q")) {
            printf("Exit...\n");
            pthread_cancel(tid);
            break;
        }
        if (send(sockfd, message, strlen(message) + 1, 0) < 0) {
            perror("send");
            exit(EXIT_FAILURE);
        }
    }

    pthread_join(tid, NULL);
    close(sockfd);
    return 0;
}
