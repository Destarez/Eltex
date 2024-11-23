#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_LENGTH_STRING 50
#define MAX_AMOUNT_ARGS 10


int main()
{   
    char input_string[MAX_LENGTH_STRING]={0};
    for(;;)
    {   
        printf(">> ");

        if (fgets(input_string,MAX_LENGTH_STRING,stdin) != 0)
        {
            if (input_string[strlen(input_string)-1] == '\n')
            {
                input_string[strlen(input_string)-1] = '\0';
            }
        }

        if (strcmp(input_string,"q")==0)
        {
            break;
        }


        char *arg = strtok(input_string," ");
        char *args[MAX_AMOUNT_ARGS];
        int i = 0;
        while( i < MAX_AMOUNT_ARGS && arg!=NULL) {
            args[i] = arg;
            arg = strtok(NULL," ");
             i++;
        }
        args[i]=NULL;
        pid_t pid;
    
        switch (pid = fork()) {
        case -1:
            perror("fork");
            exit(EXIT_FAILURE);
        case 0:
            execv(args[0],args);
            puts("Command is undefined");
            exit(EXIT_FAILURE);
        }
        wait(NULL);
    }
    exit(EXIT_SUCCESS);
}

