#ifndef PEOPLE_H
#define PEOPLE_H

#define MAX_CHAR_LENGTH 50

// Структуры
typedef struct {
    char workplace[MAX_CHAR_LENGTH];
    char post[MAX_CHAR_LENGTH];
} job;

typedef struct {
    char personalPhone[MAX_CHAR_LENGTH];
    char workPhone[MAX_CHAR_LENGTH];
} contacts;

typedef struct {
    int id;
    char name[MAX_CHAR_LENGTH];
    char surname[MAX_CHAR_LENGTH];
    char patronymic[MAX_CHAR_LENGTH];
    char email[MAX_CHAR_LENGTH];
    char socialMedia[MAX_CHAR_LENGTH];
    job occupation;
    contacts phoneNumber;
} person;

typedef struct Node {
    void *value;
    struct Node *next;
    struct Node *prev;
} Node;

typedef struct List {
    Node *head;
    Node *tail;
    int count;
} List;

// Функции
void ClearNewline(char *str);
List* createList();
void deleteList(List *list);
void printList(List *list);
void pushBack(List *list, void *data);
void* popFront(List *list);
void insertBeforeElement(List *list, Node *nextNode, void *data);
int compareSurname(void *a, void *b);
void insertionSort(List **list, int (*cmp)(void*, void*));
void editContact(person *p);
void deletePerson(List *list, char* lastname);
person* createPerson(int id);

#endif
