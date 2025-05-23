#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>


#define MAX_CHAR 256

struct
{
    long mtype;
    char mtext[MAX_CHAR];
}message;

int main()
{
    key_t key = ftok("chatkey", 38);
    if (key == -1) 
    {
        perror("ftok");
        exit(1);
    }

    int msgid = msgget(key, IPC_CREAT | 0666);
    if (msgid == -1) 
    {
        perror("msgget");
        exit(1);
    }

    while(1)
    {
        printf("User1:");
        fgets(message.mtext, MAX_CHAR, stdin);
        if (strlen(message.mtext) > 0 && message.mtext[strlen(message.mtext) - 1] == '\n')
        {
            message.mtext[strlen(message.mtext) - 1] = '\0';
        }

        if (strcmp(message.mtext, "exit") == 0) 
        {
            message.mtype = 10;
            printf("User1 left.\n");
            msgsnd(msgid, &message, strlen(message.mtext) + 1, 0);
            break;
        }

        message.mtype = 1;
        msgsnd(msgid, &message, strlen(message.mtext) + 1, 0);

        msgrcv(msgid, &message, sizeof(message.mtext), 0, 0);

        if (message.mtype == 10) 
        {
            printf("User2: %s\n", message.mtext);
            printf("User2 left.\n");
            break;
        }

        if (message.mtype == 2) 
        {
            printf("User2: %s\n", message.mtext);
        }
        
    }
    msgctl(msgid, IPC_RMID, NULL);
    return 0;
}