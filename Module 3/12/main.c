#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>


#define MAX_CHAR_LENGTH 64

pid_t pid;
int shmid;
int *shm_numbers;
int done = 0;

void sigint_handler(int sig) 
{

    if (pid > 0) 
    {
        kill(pid, SIGINT);
        wait(NULL);
        shmdt(shm_numbers);
        shmctl(shmid, IPC_RMID, NULL);
        printf("\nAmount: %d\n", done);
    }
    exit(0); 
}

int RandomNumbers(int count, int *shm_numbers)
{   
    srand(time(NULL));

    for (int i = 1; i <= count; i++) 
    {
        shm_numbers[i] = rand()%100;
    }

    return 0;
}

int FindMin(int *shm_numbers)
{
    int min=shm_numbers[1];
    for (int i = 1; i<=shm_numbers[0]; i++)
    {
        if (shm_numbers[i]<min) min = shm_numbers[i];
    }
    shm_numbers[shm_numbers[0]+1] = min;

    return 0;
}

int FindMax(int *shm_numbers)
{
    int max=shm_numbers[1];
    for (int i = 1; i<=shm_numbers[0]; i++)
    {
        if (shm_numbers[i]>max) max = shm_numbers[i];
    }
    shm_numbers[shm_numbers[0]+2] = max;

    return 0;
}

int main() 
{
    signal(SIGINT, sigint_handler);

    key_t key = ftok("shmkey", 38);

    shmid = shmget(key, 1024, IPC_CREAT | 0666);
    if (shmid == -1) {
        perror("shmget");
        exit(1);
    }

    shm_numbers = (int *) shmat(shmid, NULL, 0);
    if (shm_numbers == (int *) -1) {
        perror("shmat");
        exit(1);
    }

    switch(pid=fork())
    {
        case -1:
        perror("fork");
        exit(1);

        case 0:
        {
            printf("Child process\n");
        while (1) 
        {
            FindMin(shm_numbers);
            FindMax(shm_numbers);
            sleep(1);

        }
        shmdt(shm_numbers);
        exit(0);
        }
        
        default:
        while(1)
        {   
            printf("Parent process\n");
            int count = 2 + rand()%5;
            shm_numbers[0]=count;
            RandomNumbers(count, shm_numbers);

            for(int i = 1; i <= count; i++){
                printf("%d generated number:%d\n", i,shm_numbers[i]);
            }
            sleep(3);
            printf("Min: %d\nMax: %d\n", shm_numbers[count+1], shm_numbers[count+2]);
            done++;
        }
    }

        
    return 0;
}