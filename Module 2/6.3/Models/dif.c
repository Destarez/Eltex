#include <stdio.h>
#include <stdarg.h>


#define MAX_CHAR_LENGHT 50

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