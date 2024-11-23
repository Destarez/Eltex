#include <stdio.h>
#include <stdarg.h>
#include "process.h"

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