#include <stdio.h>
#include <stdarg.h>

double calculate(int n, ...)
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