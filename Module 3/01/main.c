#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int Area(int side_lengths) {
    return side_lengths * side_lengths;
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Usage: %s side1 side2 ...\n", argv[0]);
        return 1;
    }
    int half_argc = (argc-1)/2;
    pid_t pid;

    int *side_lengths = (int *)malloc((argc - 1) * sizeof(int));
        if (side_lengths == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }
    
        for (int i = 0; i < argc - 1; i++) {
            side_lengths[i] = atoi(argv[i + 1]);
        }

    switch(pid=fork())
    {
        case -1:
        perror("fork");
        exit(1);

        case 0:
        
        printf("Дочерний процесс: \n");
        for (int i = 0; i < half_argc; i++) {
            printf("Площадь квадрата №%d: %d\n", i + 1, Area(side_lengths[i]));
        }
        break;
        
        default:
        
        printf("Родительский процесс: \n");
        for (int i = half_argc; i < argc-1; i++) {
            printf("Площадь квадрата №%d: %d\n", i + 1, Area(side_lengths[i]));
        }
        break;
        exit(EXIT_SUCCESS);
        }

        
    free(side_lengths);
    return 0;
}
