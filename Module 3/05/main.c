#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_CHAR_LENGTH 64

volatile sig_atomic_t access_flag = 0; 

void sigusr1_handler(int signum) {
    access_flag = 1; 
}

void sigusr2_handler(int signum) {
    access_flag = 0; 
}

int RandomNumbers(int count, int *numbers)
{   
    srand(time(NULL));

    for (int i = 0; i < count; i++) {
        numbers[i] = rand();
    }
}

int main(int argc, char *argv[]) 
{

    signal(SIGUSR1, sigusr1_handler);
    signal(SIGUSR2, sigusr2_handler);

    int p[2];
    if (pipe(p))
        {
            perror("ошибка канала");
            exit(1);
        }
    
    pid_t pid;


    int count = atoi(argv[1]);

    int *numbers = (int *)malloc(count * sizeof(int));
        if (numbers == NULL) {
            perror("");
            return 1;
        }

    switch(pid=fork())
    {
        case -1:
        perror("fork");
        free(numbers);
        exit(1);

        case 0:
        {
        close(p[0]);

        RandomNumbers(count, numbers);

        

        write(p[1], numbers, count * sizeof(int));

        while (!access_flag) 
        {
            pause(); 
        }

        close(p[1]);

        sleep(3);

        FILE *file = fopen("numbers.txt", "r");
        char digits[MAX_CHAR_LENGTH];   
        while (fscanf(file, "%s", digits) != EOF) 
        {   
            printf("%s\n", digits);
            sleep(1);
        }
            
        fclose(file); 


        break;
        }
        
        default:
        
        kill(pid, SIGUSR1); 
        read(p[0],numbers ,count * sizeof(int));

        FILE *file = fopen("numbers.txt", "w");

        for (int i = 0; i < count; i++) {
            fprintf(file, "%d\n", numbers[i]);
        }
        fclose(file);
        kill(pid,SIGUSR2);
        
        close(p[0]);
        close(p[1]); 

        wait(NULL);
        break;
    }

        
    free(numbers);
    return 0;
}