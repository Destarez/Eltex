#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_CHAR_LENGTH 50

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

    List *out = createList();
    pushBack(out, popFront(*list));

    Node *unsorted = (*list)->head;
    while (unsorted) {
        Node *sorted = out->head;
        void *data = popFront(*list);

        while (sorted && cmp(data, sorted->value) > 0) {
            sorted = sorted->next;
        }

        if (sorted) {
            insertBeforeElement(out, sorted, data);
        } else {
            pushBack(out, data);
        }

        unsorted = (*list)->head;
    }

    free(*list);
    *list = out;
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

int main() {
    List *peopleList = createList();
    int choice, id = 1;

    while (1) {
        printf("\nMain Menu\n");
        printf("1. Add person\n");
        printf("2. View people\n");
        printf("3. Edit contact by surname\n");
        printf("4. Delete person by surname\n");
        printf("5. Delete entire list\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                person *newPerson = createPerson(id++);

                printf("Enter name: ");
                fgets(newPerson->name, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->name);

                printf("Enter surname: ");
                fgets(newPerson->surname, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->surname);

                printf("Enter patronymic (optional): ");
                fgets(newPerson->patronymic, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->patronymic);

                printf("Enter email (optional): ");
                fgets(newPerson->email, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->email);

                printf("Enter social media link (optional): ");
                fgets(newPerson->socialMedia, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->socialMedia);

                printf("Enter workplace (optional): ");
                fgets(newPerson->occupation.workplace, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->occupation.workplace);

                printf("Enter job post (optional): ");
                fgets(newPerson->occupation.post, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->occupation.post);

                printf("Enter personal phone (optional): ");
                fgets(newPerson->phoneNumber.personalPhone, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->phoneNumber.personalPhone);

                printf("Enter work phone (optional): ");
                fgets(newPerson->phoneNumber.workPhone, MAX_CHAR_LENGTH, stdin);
                ClearNewline(newPerson->phoneNumber.workPhone);

                pushBack(peopleList, newPerson);
                insertionSort(&peopleList, compareSurname);
                printf("List automatically sorted by surname.\n");
                break;
            }
            case 2:
                printList(peopleList);
                break;

            case 3: {
                printf("Enter surname of person to edit: ");
                char lastname[MAX_CHAR_LENGTH];
                fgets(lastname, MAX_CHAR_LENGTH, stdin);
                ClearNewline(lastname);
                
                Node *current = peopleList->head;
                while (current) {
                    person *p = (person*) current->value;
                    if (strcmp(p->surname, lastname) == 0) {
                        editContact(p);
                        break;
                    }
                    current = current->next;
                }
                if (!current) {
                    printf("Person not found.\n");
                }
                break;
            }
            case 4: {
                printf("Enter surname of person to delete: ");
                char lastname[MAX_CHAR_LENGTH];
                fgets(lastname, MAX_CHAR_LENGTH, stdin);
                ClearNewline(lastname);
                deletePerson(peopleList, lastname);
                break;
            }
            case 5:
                deleteList(peopleList);
                peopleList = createList();
                break;

            case 6:
                deleteList(peopleList);
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
