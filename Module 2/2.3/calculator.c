#include <stdio.h>
#include <stdarg.h>

#define MAX_CHAR_LENGHT 50

double sum(int n, ...)
{
    double sumResult = 0;
    va_list factor;         //указатель va_list
    va_start(factor, n);    // устанавливаем указатель
    for(int i=0;i<n; i++)
    {
        sumResult += va_arg(factor, double);  // получаем значение текущего параметра типа int
    }
    va_end(factor); // завершаем обработку параметров
    return sumResult;
}

double dif(int n, ...)
{
    va_list factor;
    va_start(factor, n);
    double difResult = va_arg(factor, double);
    for(int i=1;i<n; i++)
    {
        difResult -= va_arg(factor, double);
    }
    va_end(factor);
    return difResult;
}

double multiply(int n, ...)
{
    va_list factor;
    va_start(factor, n);
    double multiplyResult = va_arg(factor, double);
    for(int i=1;i<n; i++)
    {
        multiplyResult *= va_arg(factor, double);
    }
    va_end(factor);
    return multiplyResult;
}

double div(int n, ...)
{
    va_list factor;
    va_start(factor, n);
    double divResult = va_arg(factor, double);
    for (int i = 1; i < n; i++)
    {
        double divisor = va_arg(factor, double);
        if (divisor != 0)
            divResult /= divisor;
        else
        {
            printf("Error: Division by zero!\n");
            va_end(factor);
            return 0;
        }
    }
    va_end(factor);
    return divResult;
}

typedef struct
{
    char name[MAX_CHAR_LENGHT];
    double (*func)(int n, ...);
}Operation;

double process(int operation)
{
    Operation operations[] = 
    {
        {"add", sum},
        {"subtract", dif},
        {"multiply", multiply},
        {"divide", div}
    };
    int count;
    printf("Enter amount of numbers: ");
    scanf("%d", &count);
    if (count>=2 && count<=3)
    {
        double numbers[count];
        printf("Enter %d numbers\n", count);
        for( int i = 0; i < count ; i++)
        {
            scanf("%lf", &numbers[i]);
        }
        double result = 0;
        
        switch (count)
        {
            case 2:
                result = operations[operation - 1].func(count,numbers[0],numbers[1]);
                break;
            case 3:
                result = operations[operation - 1].func(count,numbers[0],numbers[1],numbers[2]);
                break;
        }
    }
    else 
    {
        printf("Wrong amount of numbers\n");
    }
};

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