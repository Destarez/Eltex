#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct {
    const char* name;
    double (*func)(double, double);
} Operation;

double sum(double a, double b) { return a + b; }
double division(double a, double b) { return b == 0 ? 0 : a / b; }
double multiplication(double a, double b) { return a * b; }
double subtraction(double a, double b) { return a - b; }

void calculate(int newsockfd) {
    Operation operations[] = {
        {"+", sum},
        {"/", division},
        {"*", multiplication},
        {"-", subtraction},
    };

    double a, b, result;
    char op[2];
    char buffer[1024];

    while (1) {
        strcpy(buffer, "Enter a +-*/ b\n");
        if (write(newsockfd, buffer, strlen(buffer) + 1) < 0) break;

        int res = read(newsockfd, buffer, sizeof(buffer));
        if (res <= 0) break;

        if (sscanf(buffer, "%lf %1s %lf", &a, op, &b) != 3) {
            strcpy(buffer, "Incorrect data\n");
            write(newsockfd, buffer, strlen(buffer) + 1);
            continue;
        }

        int found = 0;
        for (int i = 0; i < sizeof(operations) / sizeof(operations[0]); ++i) {
            if (strcmp(op, operations[i].name) == 0) {
                found = 1;
                if (strcmp(op, "/") == 0 && b == 0) {
                    strcpy(buffer, "You can't divide by zero.\n");
                } else {
                    result = operations[i].func(a, b);
                    snprintf(buffer, sizeof(buffer), "Result: %.2lf\n", result);
                }
                break;
            }
        }

        if (!found) {
            strcpy(buffer, "Unknown operation\n");
        }

        write(newsockfd, buffer, strlen(buffer) + 1);
    }

    close(newsockfd);
}

int main(int argc, char* argv[]) {
    int sockfd, newsockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t clilen;
    pid_t pid;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[1]));
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (listen(sockfd, 5) < 0) {
        perror("listen");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("TCP calculator server started on port %s...\n", argv[1]);

    while (1) {
        clilen = sizeof(cliaddr);
        newsockfd = accept(sockfd, (struct sockaddr*)&cliaddr, &clilen);
        if (newsockfd < 0) {
            perror("accept");
            continue;
        }

        printf("Client %s connected.\n", inet_ntoa(cliaddr.sin_addr));

        pid = fork();
        if (pid == 0) {
            close(sockfd);
            calculate(newsockfd);
            exit(0);
        } else if (pid > 0) {
            close(newsockfd);
        } else {
            perror("fork");
        }
    }

    close(sockfd);
    return 0;
}
