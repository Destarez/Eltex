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
void send_file();
void calculate();
void* recv_message(void* arg);
long long get_file_size(FILE* f);

int main(int argc, char* argv[]) {
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
    int choice;
    printf("1.Отправить файл серверу\n2.Отправить пример для расчета серверу\n");
    scanf("%d", &choice);
    if (send(sockfd, &choice, sizeof(choice), 0) < 0) {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    switch (choice) {
        case 1:
            send_file();
            break;
        case 2:
            calculate();
            break;
        default:
            close(sockfd);
            break;
    }
    exit(EXIT_SUCCESS);
}

void send_file() {
    char path[100];
    puts("Введите путь к файлу");
    scanf("%s", path);
    FILE* f;
    if ((f = fopen(path, "rb")) == NULL) {
        perror("fopen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (send(sockfd, path, strlen(path) + 1, 0) < 0) {
        perror("send");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    long long file_size = get_file_size(f);
    printf("Размер файла: %lld\n", file_size);
    int size = 50000;
    char* file_fragment = malloc(size);
    int fragment_size;
    int send_bytes = 0;
    long long total_send = 0;
    int progress;
    for (;;) {
        fragment_size = fread(file_fragment, 1, size, f);
        if (fragment_size == 0)
            break;
        if ((send_bytes = send(sockfd, file_fragment, fragment_size, 0)) < 0) {
            perror("send");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        total_send += send_bytes;
        progress = (int)(double)total_send / file_size * 100;
        printf("\rПрогресс: %d%%", progress);
    }
    printf("\n");
    fclose(f);
    free(file_fragment);
    close(sockfd);
    return;
}

void calculate() {
    pthread_t tid;
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
}

long long get_file_size(FILE* f) {
    fseek(f, 0, SEEK_END);
    long long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    return size;
}

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