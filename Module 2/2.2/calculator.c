#include <stdio.h>
#include <stdarg.h>

int sum(int n, ...)
{
    int sumResult = 0;
    va_list factor;         //указатель va_list
    va_start(factor, n);    // устанавливаем указатель
    for(int i=0;i<n; i++)
    {
        sumResult += va_arg(factor, int);  // получаем значение текущего параметра типа int
    }
    va_end(factor); // завершаем обработку параметров
    return sumResult;
}

int dif(int n, ...)
{
    va_list factor;
    va_start(factor, n);
    int difResult = va_arg(factor, int);
    for(int i=1;i<n; i++)
    {
        difResult -= va_arg(factor, int);
    }
    va_end(factor);
    return difResult;
}

int multiply(int n, ...)
{
    va_list factor;
    va_start(factor, n);
    int multiplyResult = va_arg(factor, int);
    for(int i=1;i<n; i++)
    {
        multiplyResult *= va_arg(factor, int);
    }
    va_end(factor);
    return multiplyResult;
}

double div(int n, ...)
{
    va_list factor;
    va_start(factor, n);
    double divResult = va_arg(factor, double);
    for(int i=1;i<n; i++)
    {
        divResult /= va_arg(factor, double);
    }
    va_end(factor);
    return divResult;
}
int main()
{
    int choice, count;
    while (choice != 5)
    {
        printf("------------------------------------------\n");
        printf("Choose operation:\n");
        printf("1.Addition\n2.Subtraction\n3.Multiplication\n4.Division\n5.Exit\n");
        scanf("%d", &choice);
        printf("------------------------------------------\n");
        getchar();

        if(choice == 1)
        {
            printf("Enter amount of numbers: ");
            scanf("%d", &count);
            int numbers[count];
            printf("Enter %d numbers\n", count);
            for(int i = 0; i < count; i++)
            {
                printf("Enter %d number: ", i + 1);
                scanf("%d", &numbers[i]);
            }
            printf("Result: %f\n", sum(count, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6]));
        }
        
        else if (choice == 2)
        {
            printf("Enter amount of numbers: ");
            scanf("%d", &count);
            int numbers[count];
            printf("Enter %d numbers\n", count);
            for(int i = 0; i < count; i++)
            {
                printf("Enter %d number: ", i + 1);
                scanf("%d", &numbers[i]);
            }
            printf("Result: %f\n", dif(count, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6]));
        }

        else if (choice == 3)
        {
            printf("Enter amount of numbers: ");
            scanf("%d", &count);
            int numbers[count];
            printf("Enter %d numbers\n", count);
            for(int i = 0; i < count; i++)
            {
                printf("Enter %d number: ", i + 1);
                scanf("%d", &numbers[i]);
            }
            printf("Result: %f\n", multiply(count, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6]));
        }

        else if (choice == 4)
        {
            printf("Enter amount of numbers: ");
            scanf("%d", &count);
            double numbers[count];
            printf("Enter %d numbers\n", count);
            for(int i = 0; i < count; i++)
            {
                printf("Enter %d number: ", i + 1);
                scanf("%lf", &numbers[i]);
            }
            printf("Result: %f\n", div(count, numbers[0], numbers[1], numbers[2], numbers[3], numbers[4], numbers[5], numbers[6]));
        }
    }
    return 0;
}