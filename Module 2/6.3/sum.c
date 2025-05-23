#include <stdio.h>
#include <stdarg.h>

#define MAX_CHAR_LENGHT 50

double calculate(int n, ...)
{
    double sumResult = 0;
    va_list factor;         //указатель va_list
    va_start(factor, n);    // устанавливаем указатель
    for(int i=0;i<n; i++)
    {
        sumResult +=va_arg(factor, double);  // получаем значение текущего параметра типа int
    }
    va_end(factor); // завершаем обработку параметров
    return sumResult;
}