#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_CHAR 256

int main() {
    mq_unlink("/chat");

    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MAX_CHAR;
    attr.mq_curmsgs = 0;

    mqd_t mq = mq_open("/chat", O_CREAT | O_RDWR, 0666, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        exit(1);
    }

    char message[MAX_CHAR];
    char buffer[MAX_CHAR];
    unsigned int prio;

    while (1) {
        printf("User1: ");
        fgets(message, MAX_CHAR, stdin);
        if((message[strlen(message) - 1])=='\n')
        {
            message[strlen(message) - 1] = '\0';
        }

        unsigned int send_prio = 0;
        if (strcmp(message, "exit") == 0) {
            send_prio = 10;
        }

        if (mq_send(mq, message, strlen(message) + 1, send_prio) == -1) {
            perror("mq_send");
            exit(1);
        }

        if (send_prio == 10) {
            printf("User1 left\n");
            break;
        }

        ssize_t bytes_received = mq_receive(mq, buffer, MAX_CHAR, &prio);
        if (bytes_received == -1) {
            perror("mq_receive");
            exit(1);
        }

        if (prio == 10) {
            printf("User2 left\n");
            break;
        }

        printf("User2: %s\n", buffer);
    }

    mq_close(mq);
    mq_unlink("/chat");
    return 0;
}
