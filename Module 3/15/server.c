#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
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

void calculate(int);

double sum(double a, double b) {
    return a + b;
}

double division(double a, double b) {
    if (b == 0)
        return 0;
    return a / b;
}

double multiplication(double a, double b) {
    return a * b;
}

double subtraction(double a, double b) {
    return a - b;
}

typedef struct
{
    const char *name;
    double (*func)(double, double);
} Operation;

int main(int argc, char *argv[]) {
    int sockfd;
    int newsockfd;
    pid_t pid;
    struct sockaddr_in server_addr, client_addr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("sockfd");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_port = htons(PORT);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    fd_set fds;
    int sel_fds;
    struct timeval delay;
    FD_ZERO(&fds);
    FD_SET(sockfd, &fds);
    int max_fd = sockfd;
    socklen_t clilen = sizeof(client_addr);
    for (;;) {
        delay.tv_sec = 60;
        delay.tv_usec = 0;
        if ((sel_fds = select(max_fd + 1, &fds, 0, 0, &delay)) < 0) {
            perror("select");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        if (sel_fds == 0)
            break;

        if ((newsockfd = accept(sockfd, (struct sockaddr *)&client_addr, &clilen)) < 0) {
            perror("accept");
            close(sockfd);
            exit(EXIT_FAILURE);
        }
        printf("IP %s подключился!\n", inet_ntoa(client_addr.sin_addr));
        pid = fork();
        if (pid < 0) {
            perror("fork");
            close(sockfd);
            close(newsockfd);
            exit(EXIT_FAILURE);
        }
        if (pid == 0) {
            close(sockfd);
            calculate(newsockfd);
            close(newsockfd);
            exit(EXIT_SUCCESS);
        } else
            close(newsockfd);
    }
    close(sockfd);
    exit(EXIT_SUCCESS);
}

void calculate(int newsockfd) {
    Operation operation[] = {
        {"+", sum},
        {"/", division},
        {"*", multiplication},
        {"-", subtraction},
    };
    double a, b, result;
    int res;
    char temp[2];
    char buff[MAX_MSG_SIZE];
    for (;;) {
        strcpy(buff, "Введите пример (в формате <число> <операция> <число>)\n");
        sleep(1);
        if (write(newsockfd, buff, strlen(buff) + 1) < 0) {
            perror("write");
            close(newsockfd);
            exit(EXIT_FAILURE);
        }

        if ((res = read(newsockfd, buff, sizeof(buff))) < 1) {
            if (res == 0) {
                return;
            }
            perror("read");
            close(newsockfd);
            exit(EXIT_FAILURE);
        }

        if (sscanf(buff, "%lf %s %lf", &a, temp, &b) < 3) {
            strcpy(buff, "Неправильно введенны даннные\n");
            if (write(newsockfd, buff, strlen(buff) + 1) < 0) {
                perror("write");
                close(newsockfd);
                exit(EXIT_FAILURE);
            }
            continue;
        }
        int i = 0;
        while (i < sizeof(operation) / sizeof(operation[0])) {
            if (strcmp(temp, operation[i].name) == 0) {
                result = operation[i].func(a, b);
                break;
            }
            i++;
        }

        if (result == 0 && strcmp(operation[i].name, "/") == 0) {
            strcpy(buff, "На ноль делить нельзя\n");
            if (write(newsockfd, buff, strlen(buff) + 1) < 0) {
                perror("write");
                close(newsockfd);
                exit(EXIT_FAILURE);
            }
            continue;
        }
        snprintf(buff, sizeof(buff), "%.2lf", result);
        if (write(newsockfd, buff, strlen(buff) + 1) < 0) {
            perror("write");
            close(newsockfd);
            exit(EXIT_FAILURE);
        }
    }
}