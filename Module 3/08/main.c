#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define MAX_CHAR_LENGTH 64

void RandomNumbers(int count, int *numbers)
{   
    srand(time(NULL));

    for (int i = 0; i < count; i++) {
        numbers[i] = rand();
    }
}

int main(int argc, char *argv[]) 
{
    FILE *f = fopen("semkey", "w");
    if (f == NULL) {
        perror("fopen semkey");
        exit(1);
    }
    fclose(f);

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

    key_t key = ftok("semkey", 38);
    int semid = semget(key, 1 , IPC_CREAT | 0666);

    if (semid == -1) {
        perror("semget");
        free(numbers);
        exit(1);
    }

    semctl(semid, 0, SETVAL, 0);

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

        close(p[1]);

        struct sembuf wait_op = {0, -1, 0};
        semop(semid, &wait_op, 1);

        FILE *file = fopen("numbers.txt", "r");
        if (!file) {
            perror("fopen read");
            free(numbers);
            exit(1);
        }

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
        
        read(p[0],numbers ,count * sizeof(int));

        FILE *file = fopen("numbers.txt", "w");
        if (!file) {
            perror("fopen write");
            free(numbers);
            exit(1);
        }

        for (int i = 0; i < count; i++) {
            fprintf(file, "%d\n", numbers[i]);
        }
        fclose(file);
        
        close(p[0]);
        close(p[1]); 

        struct sembuf unlock = {0, 1, 0};
        semop(semid, &unlock, 1);

        wait(NULL);
        break;
    }
    semctl(semid, 0, IPC_RMID);
        
    free(numbers);
    return 0;
}
