#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX_MSG_SIZE 1024
#define PORT 1510

int sockfd;

void* recv_message(void* arg) {
    char message2[MAX_MSG_SIZE];
    for (;;) {
        if (read(sockfd, message2, sizeof(message2)) < 1) {
            perror("recv");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("\nS=>C: %s\n", message2);
    }
    pthread_exit(EXIT_SUCCESS);
}

int main(int argc, char* argv[]) {
    pthread_t tid;
    struct sockaddr_in server_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("sockfd");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr.s_addr) < 1) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (pthread_create(&tid, NULL, recv_message, NULL) < 0) {
        perror("pthread_create");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    char message[MAX_MSG_SIZE];
    for (;;) {
        if (fgets(message, MAX_MSG_SIZE, stdin) != NULL) {
            if (message[strlen(message) - 1] == '\n') {
                message[strlen(message) - 1] = '\0';
            }
        }

        if (!strcmp(message, "q")) {
            printf("Выход..\n");
            if (pthread_cancel(tid) != 0) {
                perror("pthread_cancel");
                exit(EXIT_FAILURE);
            }
            break;
        }

        if (send(sockfd, message, strlen(message) + 1, 0) < 0) {
            perror("send");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
    }
    if (pthread_join(tid, NULL) != 0) {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    close(sockfd);
    exit(EXIT_SUCCESS);
}