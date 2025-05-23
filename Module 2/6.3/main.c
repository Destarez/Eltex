#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

typedef double (*operation_func)(int, ...);

typedef struct {
    const char *name;
    const char *lib_path;
    operation_func func;
} Operation;

int main()
{
    void *handle;
    Operation operations[] = {
        {"Addition", "./libsum.so", NULL},
        {"Subtraction", "./libdif.so", NULL},
        {"Multiplication", "./libmult.so", NULL},
        {"Division", "./libdiv.so", NULL}
    };

    int choice = 0;
    while (choice != 5)
    {
        printf("------------------------------------------\n");
        printf("Choose operation:\n");
        for (int i = 0; i < 4; i++)
        {
            printf("%d. %s\n", i + 1, operations[i].name);
        }
        printf("5. Exit\n");
        scanf("%d", &choice);
        printf("------------------------------------------\n");

        if (choice >= 1 && choice <= 4)
        {
            handle = dlopen(operations[choice - 1].lib_path, RTLD_LAZY);
            if (!handle)
            {
                fprintf(stderr, "Error loading library: %s\n", dlerror());
                return 1;
            }

            operations[choice - 1].func = (operation_func)dlsym(handle, "calculate");
            if (!operations[choice - 1].func)
            {
                fprintf(stderr, "Error loading function: %s\n", dlerror());
                dlclose(handle);
                return 1;
            }

            int count;
            printf("Enter amount of numbers: ");
            scanf("%d", &count);
            double numbers[count];
            printf("Enter %d numbers: ", count);
            for (int i = 0; i < count; i++)
            {
                scanf("%lf", &numbers[i]);
            }

            double result = operations[choice - 1].func(count, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6]);
            printf("Result: %lf\n", result);

            dlclose(handle);
        }
        else if (choice != 5)
        {
            printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
