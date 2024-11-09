#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint8_t **data;   
    int *low;         
    int *high;        
    int *count;       
    int *max;         
    int max_prio;     
    int min_prio;     
} queue;

queue *init(size_t size, int size_prio)
{
    queue *q = calloc(1, sizeof(queue));
    q->max_prio = size_prio;
    q->min_prio = 0;  

    q->data = calloc(size_prio, sizeof(void *));
    q->low = calloc(size_prio, sizeof(int));
    q->high = calloc(size_prio, sizeof(int));
    q->count = calloc(size_prio, sizeof(int));
    q->max = calloc(size_prio, sizeof(int));

    for (int i = 0; i < size_prio; i++)
    {
        q->data[i] = calloc(size, sizeof(uint8_t));
        q->low[i] = 0;    
        q->high[i] = -1;  
        q->max[i] = size; 
    }

    return q;
}

int queueAdd(queue *q, uint8_t a, int prio)
{
    
    if (q->count[prio] == q->max[prio])
    {
        printf("Not enough space in queue with priority %d\n", prio);
        return -1;
    }

    
    q->data[prio][q->low[prio]] = a;
    q->count[prio]++;

    q->low[prio] = (q->low[prio] + 1) % q->max[prio];

    if (prio < q->min_prio)
        q->min_prio = prio;

    return 0;
}

int queueGet(queue *q, uint8_t *value)
{
    uint8_t a = 0;

    for (int i = q->min_prio; i < q->max_prio; i++)
    {
        if (q->count[i] > 0)
        {
            q->high[i] = (q->high[i] + 1) % q->max[i];
            a = q->data[i][q->high[i]];
            q->count[i]--;

            if (q->count[i] == 0 && i == q->min_prio)
            {
                while (i < q->max_prio && q->count[i] == 0)
                {
                    i++;
                }
                q->min_prio = i;
            }

            *value = a;
            return 0;
        }
    }

    printf("Queue is empty\n");
    return -1;
}

int main(int argc, char **argv)
{
    queue *q = init(10, 256);

    queueAdd(q, 2, 3);
    queueAdd(q, 10, 1);
    queueAdd(q, 12, 4);
    queueAdd(q, 13, 0);

    for (int i = 1; i < 5; i++)
    {
        uint8_t d;
        int result = queueGet(q, &d);
        if (result == 0)
            printf("%d: %d\n", i, d);
    }

    return 0;
}
