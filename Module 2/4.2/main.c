#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR_LENGHT 50

typedef struct Item {
    int priority;
    char name[MAX_CHAR_LENGHT];
    char surname[MAX_CHAR_LENGHT];
    struct Item* next;
    struct Item* prev;
} Item;

typedef struct {
    Item* front;
} PriorityQueue;

PriorityQueue contactQueue = {NULL};

void ClearNewline(char *str) {
    if (strlen(str) > 0 && str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

void InsertQueue(int priority, char* surname, char* name, PriorityQueue* queue) {
    if (priority<0 || priority>255)
    {
        printf("The priority is specified by an integer from 0 to 255.");
        return;
    }
    Item* newItem = (Item*)malloc(sizeof(Item));
    if (!newItem) {
        printf("Memory error!\n");
        return;
    }

    newItem->priority = priority;
    strncpy(newItem->surname, surname, MAX_CHAR_LENGHT - 1);
    ClearNewline(newItem->surname);
    strncpy(newItem->name, name, MAX_CHAR_LENGHT - 1);
    ClearNewline(newItem->name);
    newItem->next = NULL;
    newItem->prev = NULL;
    
    if (queue->front == NULL || queue->front->priority > priority) {
        newItem->next = queue->front;
        if (queue->front != NULL) {
            queue->front->prev = newItem;
        }
        queue->front = newItem;
        return;
    }

    Item* temp = queue->front;
    while (temp->next != NULL && temp->next->priority <= priority) {
        temp = temp->next;
    }

    newItem->next = temp->next;
    if (temp->next != NULL) {
        temp->next->prev = newItem;
    }
    temp->next = newItem;
    newItem->prev = temp;
}


void PrintQueue(PriorityQueue* queue) {
    Item* temp = queue->front;
    printf("Person ??:\n");
    while (temp != NULL) {
        printf("Priority: %d, Surname: %s, Name: %s\n", temp->priority, temp->surname, temp->name);
        temp = temp->next;
    }
}

void PrintPeopleWithPriority(int priority,PriorityQueue* queue)
{
    Item* temp = queue->front;
    printf("People with %d priority:\n",priority);

    int found = 0;

    while (temp != NULL) 
    {
        if (priority==temp->priority)
        {
            printf("Priority: %d, Surname: %s, Name: %s\n", temp->priority, temp->surname, temp->name);
            found = 1;
        }
        temp = temp->next;

    }
    if (found==0)
    {
        printf("Priority %d not found in the queue.\n", priority);
    }
}

void FirstPersonInQueue(PriorityQueue* queue)
{
    if (queue->front == NULL) {
        printf("The queue is empty.\n");
        return;
    }
    Item* temp = queue->front;
    printf("First person:\n");
    printf("Priority: %d, Surname: %s, Name: %s\n", temp->priority, temp->surname, temp->name);
}

void PeoplePnH(int priority, PriorityQueue* queue)
{   
    Item* temp = queue->front;
    printf("People list:\n");

    int found = 0;

    while (temp != NULL) 
    {
        if (priority>=temp->priority)
        {
            printf("Priority: %d, Surname: %s, Name: %s\n", temp->priority, temp->surname, temp->name);
            found = 1;
        }
        temp = temp->next;

    }
    if (found==0)
    {
        printf("Priority %d not found in the queue.\n", priority);
    }
}

void DeletePerson(int priority, char* surname, char* name, PriorityQueue* queue) {
    Item* temp = queue->front;

    if (queue->front == NULL) {
        printf("Queue is empty.\n");
        return;
    }

    while (temp != NULL)
    {
        if (temp->priority==priority && strcmp(temp->surname,surname)==0 && strcmp(temp->name, name)==0)
        {
            if (temp->prev) {
                temp->prev->next = temp->next;
            } else {
                queue->front = temp->next;
            }
            if (temp->next) {
                temp->next->prev = temp->prev;
            }
            printf("Removed: Priority %d Surname %s Name %s\n", temp->priority, temp->surname, temp->name);
            free(temp);
            return;
        }
        temp = temp->next;
    }
    printf("Person not found in the queue.\n");
}

void FreeMemory(PriorityQueue* queue) {
    Item* temp = queue->front;
    while (temp) {
        Item* next = temp->next;
        free(temp);
        temp = next;
    }
    queue->front = NULL;
}

int main() {
    int choice;
    int priority; 
    char surname[MAX_CHAR_LENGHT], name[MAX_CHAR_LENGHT];

    do {
        printf("\nMenu:\n");
        printf("1. Add person\n");
        printf("2. Person list\n");
        printf("3. People with a certain priority\n");
        printf("4. First person in queue\n");
        printf("5. People with a certain priority and higher\n");
        printf("6. Delete person\n");
        printf("7. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter priority: ");
                scanf("%d",&priority);
                getchar();

                printf("Enter surname: ");
                fgets(surname, MAX_CHAR_LENGHT, stdin);
                ClearNewline(surname);

                printf("Enter name: ");
                fgets(name, MAX_CHAR_LENGHT, stdin);
                ClearNewline(name);

                InsertQueue(priority,surname, name, &contactQueue);
                break;

            case 2:
                PrintQueue(&contactQueue);
                break;

            case 3:
                printf("Enter priority: ");
                scanf("%d",&priority);
                getchar();

                PrintPeopleWithPriority(priority, &contactQueue);
                break;
            
            case 4:
                FirstPersonInQueue(&contactQueue);
                break;
            
            case 5:
                printf("Enter priority: ");
                scanf("%d",&priority);
                getchar();

                PeoplePnH(priority, &contactQueue);
                break;    
            case 6:
                printf("Enter priority: ");
                scanf("%d",&priority);
                getchar();

                printf("Enter surname: ");
                fgets(surname, MAX_CHAR_LENGHT, stdin);
                ClearNewline(surname);

                printf("Enter name: ");
                fgets(name, MAX_CHAR_LENGHT, stdin);
                ClearNewline(name);

                DeletePerson(priority,surname,name,&contactQueue);
                break;
            case 7:
                printf("Free memory and exit...\n");
                FreeMemory(&contactQueue);
                break;
            default:
                printf("ERROR. Try again.\n");
        }
    } while (choice != 7);

    return 0;
}
