#ifndef LIST_H
#define LIST_H

#define MAX_CHAR_LENGHT 50

typedef struct Item {
    char name[MAX_CHAR_LENGHT];
    char surname[MAX_CHAR_LENGHT];
    struct Item* next;
    struct Item* prev;
} Item;

typedef struct {
    Item* head;
    Item* tail;
} double_linked_list;

void ClearNewline(char *str);
void InsertSorted(char* surname, char* name, double_linked_list* list);
void PrintList(double_linked_list* list);
void DeleteItem(char* surname, char* name, double_linked_list* list);
void FreeMemory(double_linked_list* list);




#endif