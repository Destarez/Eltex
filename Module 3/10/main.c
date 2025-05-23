    #include <sys/types.h>
    #include <stdio.h>
    #include <unistd.h>
    #include <stdlib.h>
    #include <time.h>
    #include <fcntl.h>
    #include <sys/wait.h>
    #include <semaphore.h>
    #include <fcntl.h>

    #define MAX_CHAR_LENGTH 64

    int RandomNumbers(int count, int *numbers)
    {   
        srand(time(NULL));

        for (int i = 0; i < count; i++) {
            numbers[i] = rand();
        }
        return 0;
    }

    int main(int argc, char *argv[]) 
    {
        sem_unlink("/sem");
        sem_t *sem = sem_open("/sem", O_CREAT, 0644, 1);
        if (sem == SEM_FAILED) 
        {
            perror("sem_open");
            return 1;
        }

        int p[2];
        if (pipe(p))
            {
                perror("pipe");
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

            close(p[1]);

            sleep(3);
            
            sem_wait(sem);
            FILE *file = fopen("numbers.txt", "r");
            if (file == NULL) 
            {
                perror("fopen (child)");
                sem_post(sem);
                exit(1);
            }
            char digits[MAX_CHAR_LENGTH];   
            while (fscanf(file, "%s", digits) != EOF) 
            {   
                printf("%s\n", digits);
                sleep(1);
            }
                
            fclose(file); 
            sem_post(sem);

            break;
            }
            
            default:

            sem_wait(sem);

            close(p[1]);
        
            read(p[0],numbers ,count * sizeof(int));

            close(p[0]);

            FILE *file = fopen("numbers.txt", "w");
            if (file == NULL)
            {
                perror("fopen (parent)");
                sem_post(sem);
                exit(1);
            }

            for (int i = 0; i < count; i++) {
                fprintf(file, "%d\n", numbers[i]);
            }
            fclose(file);
            sem_post(sem);

            wait(NULL);


            sem_unlink("/sem");
            sem_close(sem);

        }

            
        free(numbers);
        return 0;
    }