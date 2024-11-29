#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

int RandomNumbers(int count, int *numbers)
{   
    srand(time(NULL));

     for (int i = 0; i < count; i++) {
        numbers[i] = rand();
    }
}

int main(int argc, char *argv[]) {
    int p[2];
    if (pipe(p))
        {
            perror("Ошибка канала");
            exit(1);
        }
    
    pid_t pid;


    int count = atoi(argv[1]);

    int *numbers = (int *)malloc(count * sizeof(int));
        if (numbers == NULL) {
            perror("Ошибка памяти");
            return 1;
        }

    switch(pid=fork())
    {
        case -1:
        perror("fork");
        free(numbers);
        exit(1);

        case 0:
        close(p[0]);

        RandomNumbers(count, numbers);

        write(p[1], numbers, count * sizeof(int));

        close(p[1]);

        break;
        
        default:
        
        
        read(p[0],numbers ,count * sizeof(int)); 

        printf("Родительский процесс: \n");

        for ( int i = 0; i < count; i++)
        {
            printf("Число %d: %d\n",i+1,numbers[i]);
        }

        FILE *file = fopen("numbers.txt", "w");

        for (int i = 0; i < count; i++) {
            fprintf(file, "%d\n", numbers[i]);
        }

        fclose(file);
        
        close(p[0]);
        close(p[1]); 

        break;
        exit(EXIT_SUCCESS);
        }

        
    free(numbers);
    return 0;
}