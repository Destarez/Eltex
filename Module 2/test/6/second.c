#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

struct message {
    long mtype;
    char mtext[100];
};

int main() {
    key_t key = ftok(".", 'A');
    if (key == -1) {
        perror("ftok");
        exit(1);
    }

    int msgid = msgget(key, 0666 | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget");
        exit(1);
    }

    struct message msg;
    int running = 1;

    printf("Enter message for First: ");
    fflush(stdout);

    memset(&msg, 0, sizeof(msg));
    fgets(msg.mtext, sizeof(msg.mtext), stdin);

    if (strlen(msg.mtext) > 1) {
        msg.mtype = 2;
        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }
    }

    while (running) {
        memset(&msg, 0, sizeof(msg));

        if (msgrcv(msgid, &msg, sizeof(msg.mtext), 1, 0) == -1) {
            perror("msgrcv");
            exit(1);
        }

        printf("Catched from First: %s", msg.mtext);
        fflush(stdout);

        if (msg.mtype == 255) {
            printf("First terminated chat\n");
            break;
        }

        printf("Enter message for First: ");
        fflush(stdout);

        memset(&msg, 0, sizeof(msg));
        fgets(msg.mtext, sizeof(msg.mtext), stdin);

        if (strlen(msg.mtext) <= 1) continue;

        msg.mtype = 2;
        if (msgsnd(msgid, &msg, sizeof(msg.mtext), 0) == -1) {
            perror("msgsnd");
            exit(1);
        }

        if (strncmp(msg.mtext, "end", 3) == 0) {
            msg.mtype = 255;
            strcpy(msg.mtext, "Chat terminated\n");
            msgsnd(msgid, &msg, sizeof(msg.mtext), 0);
            running = 0;
        }
    }

    return 0;
}
