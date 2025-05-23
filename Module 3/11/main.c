#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>

#define MAX_CHAR_LENGTH 64
#define MAX_READERS 3

int RandomNumbers(int count, int *numbers)
    {   
        srand(time(NULL));

        for (int i = 0; i < count; i++) {
            numbers[i] = rand();
        }
        return 0;
    }

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <count>\n", argv[0]);
        return 1;
    }

    int count = atoi(argv[1]);
    int *numbers = malloc(count * sizeof(int));
    if (!numbers) {
        perror("malloc");
        return 1;
    }

    sem_unlink("/write_sem");
    sem_unlink("/read_count_sem");
    sem_t *write_sem = sem_open("/write_sem", O_CREAT, 0644, 1);
    sem_t *read_count_sem = sem_open("/read_count_sem", O_CREAT, 0644, 1);

    if (write_sem == SEM_FAILED || read_count_sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }

 
    int *reader_count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (reader_count == MAP_FAILED) {
        perror("mmap");
        return 1;
    }
    *reader_count = 0;


    int p[2];
    if (pipe(p) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid == 0) {

        close(p[0]);
        RandomNumbers(count, numbers);
        write(p[1], numbers, count * sizeof(int));
        close(p[1]);
        exit(0);
    }


    close(p[1]);
    read(p[0], numbers, count * sizeof(int));
    close(p[0]);


    sem_wait(write_sem);
    FILE *file = fopen("numbers.txt", "w");
    if (!file) {
        perror("fopen (write)");
        sem_post(write_sem);
        return 1;
    }
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n", numbers[i]);
    }
    fclose(file);
    sem_post(write_sem);


    for (int i = 0; i < MAX_READERS; i++) {
        if (fork() == 0) 
        {
            sem_wait(read_count_sem);
            (*reader_count)++;
            if (*reader_count == 1)
                sem_wait(write_sem);
            if (reader_count == 1)
                sem_wait(write_sem);
            sem_post(read_count_sem);

            FILE *file = fopen("numbers.txt", "r");
            if (!file) {
                perror("fopen (read)");
            } else {
                char buffer[MAX_CHAR_LENGTH];
                while (fgets(buffer, sizeof(buffer), file)) {
                    printf("Reader %d read: %s", i + 1, buffer);
                }
                fclose(file);
            }

            sem_wait(read_count_sem);
            (*reader_count)--;
            if (*reader_count == 0)
                sem_post(write_sem);
            if (reader_count == 0)
                sem_post(write_sem);
            sem_post(read_count_sem);

            exit(0);
        }
    }


    while (wait(NULL) > 0);

    free(numbers);
    sem_close(write_sem);
    sem_close(read_count_sem);
    sem_unlink("/write_sem");
    sem_unlink("/read_count_sem");

    munmap(reader_count, sizeof(int));

    return 0;
}
