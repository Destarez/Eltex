#include "people.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void ClearNewline(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}

List* createList() {
    List *tmp = (List*) malloc(sizeof(List));
    tmp->head = tmp->tail = NULL;
    tmp->count = 0;
    return tmp;
}

void deleteList(List *list) {
    Node *current = list->head;
    while (current) {
        Node *next = current->next;
        free(current->value);
        free(current);
        current = next;
    }
    free(list);
    printf("List deleted.\n");
}

void printList(List *list) {
    Node *item = list->head;
    while (item) {
        person *p = (person*) item->value;
        printf("Surname: %s, Name: %s\n",p->surname, p->name);
        printf("  Patronymic: %s\n", p->patronymic);
        printf("  Email: %s\n", p->email);
        printf("  Social Media: %s\n", p->socialMedia);
        printf("  Job: %s at %s\n", p->occupation.post, p->occupation.workplace);
        printf("  Personal Phone: %s, Work Phone: %s\n", p->phoneNumber.personalPhone, p->phoneNumber.workPhone);
        item = item->next;
    }
}

void pushBack(List *list, void *data) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    if (tmp == NULL) {
        exit(1);
    }
    tmp->value = data;
    tmp->next = NULL;
    tmp->prev = list->tail;
    if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;
    if (list->head == NULL) {
        list->head = tmp;
    }
    list->count++;
}

void* popFront(List *list) {
    if (list->head == NULL) return NULL;

    Node *nodeToRemove = list->head;
    void *data = nodeToRemove->value;

    list->head = list->head->next;
    if (list->head) {
        list->head->prev = NULL;
    } else {
        list->tail = NULL;
    }

    free(nodeToRemove);
    list->count--;
    return data;
}

void insertBeforeElement(List *list, Node *nextNode, void *data) {
    Node *newNode = (Node*) malloc(sizeof(Node));
    newNode->value = data;
    newNode->next = nextNode;
    newNode->prev = nextNode->prev;

    if (nextNode->prev) {
        nextNode->prev->next = newNode;
    } else {
        list->head = newNode;
    }
    nextNode->prev = newNode;
    list->count++;
}

int compareSurname(void *a, void *b) {
    person *p1 = (person*) a;
    person *p2 = (person*) b;
    return strcmp(p1->surname, p2->surname);
}

void insertionSort(List **list, int (*cmp)(void*, void*)) {
    if ((*list)->head == NULL) return;

    Node *sorted = NULL; // Will be the sorted part of the list.
    Node *current = (*list)->head;
    
    while (current) {
        Node *next = current->next;  // Save the next node to continue iterating
        Node *sortedPos = sorted;

        // Find the position where current node needs to be inserted
        while (sortedPos && cmp(current->value, sortedPos->value) > 0) {
            sortedPos = sortedPos->next;
        }

        // Insert current node in the sorted part
        if (sortedPos == NULL) {
            // If no valid position, push it to the end of the sorted list
            if (sorted == NULL) {
                sorted = current; // First node in the sorted list
            } else {
                Node *tail = sorted;
                while (tail->next) tail = tail->next;
                tail->next = current;
                current->prev = tail;
            }
        } else {
            // Insert before sortedPos
            if (sortedPos->prev) {
                sortedPos->prev->next = current;
                current->prev = sortedPos->prev;
            } else {
                sorted = current;
            }
            current->next = sortedPos;
            sortedPos->prev = current;
        }

        current = next;  // Move to next node
    }

    // Update the original list to point to sorted list
    (*list)->head = sorted;
    (*list)->tail = sorted;
    while ((*list)->tail && (*list)->tail->next) {
        (*list)->tail = (*list)->tail->next;
    }
}

void editContact(person *p) {
    printf("Editing contact %s %s\n", p->surname, p->name);

    printf("Enter new personal phone (or press Enter to keep '%s'): ", p->phoneNumber.personalPhone);
    char newPersonalPhone[MAX_CHAR_LENGTH];
    fgets(newPersonalPhone, MAX_CHAR_LENGTH, stdin);
    ClearNewline(newPersonalPhone);
    if (strlen(newPersonalPhone) > 0) {
        strncpy(p->phoneNumber.personalPhone, newPersonalPhone, MAX_CHAR_LENGTH);
    }

    printf("Enter new work phone (or press Enter to keep '%s'): ", p->phoneNumber.workPhone);
    char newWorkPhone[MAX_CHAR_LENGTH];
    fgets(newWorkPhone, MAX_CHAR_LENGTH, stdin);
    ClearNewline(newWorkPhone);
    if (strlen(newWorkPhone) > 0) {
        strncpy(p->phoneNumber.workPhone, newWorkPhone, MAX_CHAR_LENGTH);
    }

    printf("Contact updated.\n");
}

void deletePerson(List *list, char* lastname) {
    Node *current = list->head;
    while (current) {
        person *p = (person*) current->value;
        if (strcmp(p->surname, lastname) == 0) {
            if (current->prev) {
                current->prev->next = current->next;
            } else {
                list->head = current->next;
            }
            if (current->next) {
                current->next->prev = current->prev;
            } else {
                list->tail = current->prev;
            }
            free(current->value);
            free(current);
            list->count--;
            printf("Person %s deleted.\n", lastname);
            return;
        }
        current = current->next;
    }
    printf("Person with surname %s not found.\n", lastname);
}

person* createPerson(int id) {
    person *newPerson = (person*) malloc(sizeof(person));
    newPerson->id = id;
    return newPerson;
}