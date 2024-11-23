#ifndef PROCESS_H
#define PROCESS_H
#define MAX_CHAR_LENGHT 50

typedef struct
{
    char name[MAX_CHAR_LENGHT];
    double (*func)(int n, ...);
}Operation;

double process(int operation);

#endif