#include <stdio.h>
#include <stdarg.h>

#define MAX_CHAR_LENGHT 50

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