#include <stdio.h>
#include <stdlib.h>

int Area(int side_lengths) {
    return side_lengths * side_lengths;
}

int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Usage: %s side1 side2 ...\n", argv[0]);
        return 1;
    }

    int *side_lengths = (int *)malloc((argc - 1) * sizeof(int));
        if (side_lengths == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }
    
        for (int i = 0; i < argc - 1; i++) {
            side_lengths[i] = atoi(argv[i + 1]);
        }

    for (int i = 0; i < argc - 1; i++)  
    {
        int total = Area(side_lengths[i]);
        printf("Area of %d square is %d\n", i + 1, total);
    }
    free(side_lengths);
    return 0;
}
