#include <stdio.h>
#include <stdlib.h>

int sum(int *numbers, int count)
{
    int total = 0;
    for (int i = 0; i<count; i++)
    {
        total += numbers[i];
    }
    return total;
}
int main(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Usage: %s number1 number2 ...\n", argv[0]);
        return 1;
    }

    int *numbers = (int *)malloc((argc - 1) * sizeof(int));
        if (numbers == NULL) {
            perror("Failed to allocate memory");
            return 1;
        }
    
        for (int i = 0; i < argc - 1; i++) {
            numbers[i] = atoi(argv[i + 1]);
        }
    int total = sum(numbers, argc - 1);
    printf("The sum of the numbers is: %d\n", total);

    free(numbers);
    return 0;
}
