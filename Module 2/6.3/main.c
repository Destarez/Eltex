#include <stdio.h>
#include <stdarg.h>
#include "Models/process/process.h"

int main()
{
    int choice=0;
    while (choice!=5)
    {
        printf("------------------------------------------\n");
        printf("Choose operation:\n");
        printf("1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n");
        scanf("%d", &choice);
        printf("------------------------------------------\n");
        getchar();
        if (choice >= 1 && choice <= 4)
        {
            double result = process(choice);
            printf("%lf",result);            
        }
        else if (choice != 5)
        {
            printf("Invalid choice!\n");
        }
    }
    return 0;
}