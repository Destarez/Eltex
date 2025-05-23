#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

double_linked_list contactList = {NULL, NULL};

void ClearNewline(char *str) {
    if (strlen(str) > 0 && str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

void InsertSorted(char* surname, char* name, double_linked_list* list) {
    Item* newItem = (Item*)malloc(sizeof(Item));
    if (!newItem) {
        printf("Memory error!\n");
        return;
    }

    strncpy(newItem->surname, surname, MAX_CHAR_LENGHT - 1);
    newItem->surname[MAX_CHAR_LENGHT - 1] = '\0';
    strncpy(newItem->name, name, MAX_CHAR_LENGHT - 1);
    newItem->name[MAX_CHAR_LENGHT - 1] = '\0';
    
    newItem->next = NULL;
    newItem->prev = NULL;

    if (list->head == NULL) {
        list->head = list->tail = newItem;
        return;
    }

    Item* temp = list->head;
    while (temp != NULL && strcmp(surname, temp->surname) > 0) {
        temp = temp->next;
    }

    if (temp == list->head) {
        newItem->next = list->head;
        list->head->prev = newItem;
        list->head = newItem;
    } else if (temp == NULL) {
        list->tail->next = newItem;
        newItem->prev = list->tail;
        list->tail = newItem;
    } else {
        newItem->next = temp;
        newItem->prev = temp->prev;
        temp->prev->next = newItem;
        temp->prev = newItem;
    }
}

void PrintList(double_linked_list* list) {
    Item* temp = list->head;
    printf("Person list:\n");
    while (temp != NULL) {
        printf("Surname: %s, Name: %s\n", temp->surname, temp->name);
        temp = temp->next;
    }
}

void DeleteItem(char* surname, char* name, double_linked_list* list) {
    Item* temp = list->head;

    while (temp != NULL) {
        if (strcmp(temp->surname, surname) == 0 && strcmp(temp->name, name) == 0) {
            if (temp->prev) temp->prev->next = temp->next;
            if (temp->next) temp->next->prev = temp->prev;
            if (temp == list->head) list->head = temp->next;
            if (temp == list->tail) list->tail = temp->prev;
            free(temp);
            printf("Person deleted.\n");
            return;
        }
        temp = temp->next;
    }
    printf("Person not found.\n");
}

void FreeMemory(double_linked_list* list) {
    Item* temp = list->head;
    while (temp) {
        Item* next = temp->next;
        free(temp);
        temp = next;
    }
    list->head = NULL;
    list->tail = NULL;
}

int main() {
    int choice;
    char surname[MAX_CHAR_LENGHT], name[MAX_CHAR_LENGHT];

    do {
        printf("\nMenu:\n");
        printf("1. Add person\n");
        printf("2. Person list\n");
        printf("3. Delete person\n");
        printf("4. Exit\n");
        printf("Choose option: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter surname: ");
                fgets(surname, MAX_CHAR_LENGHT, stdin);
                ClearNewline(surname);

                printf("Enter name: ");
                fgets(name, MAX_CHAR_LENGHT, stdin);
                ClearNewline(name);

                InsertSorted(surname, name, &contactList);
                break;

            case 2:
                PrintList(&contactList);
                break;

            case 3:
                printf("Enter surname: ");
                fgets(surname, MAX_CHAR_LENGHT, stdin);
                ClearNewline(surname);

                printf("Enter name: ");
                fgets(name, MAX_CHAR_LENGHT, stdin);
                ClearNewline(name);

                DeleteItem(surname, name, &contactList);
                break;

            case 4:
                printf("Free memory and exit...\n");
                FreeMemory(&contactList);
                break;

            default:
                printf("ERROR. Try again.\n");
        }
    } while (choice != 4);

    return 0;
}
