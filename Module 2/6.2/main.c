#include "people.h"
#include <stdio.h>
#include <string.h>
#include <dlfcn.h>

int main() {
    List *peopleList = createList();
    int choice, id = 1;

    void *handle;
    char *error;

    // Define function pointers for operations
    typedef person* (*createPersonFunc)(int id);
    typedef void (*addPersonFunc)(List *peopleList, person *newPerson);
    typedef void (*printListFunc)(List *peopleList);
    typedef void (*editContactFunc)(person *p);
    typedef void (*deletePersonFunc)(List *peopleList, const char *lastname);
    typedef void (*deleteListFunc)(List *peopleList);
    typedef void (*insertionSortFunc)(List **peopleList, int (*compare)(const void *, const void *));
    
    // Open the dynamic library
    handle = dlopen("libdynamic.so", RTLD_LAZY);
    
    if (!handle) {
        fprintf(stderr, "%s\n", dlerror());
        return 1;
    }

    // Get function pointers from the shared library
    createPersonFunc createPerson = (createPersonFunc) dlsym(handle, "createPerson");
    addPersonFunc addPerson = (addPersonFunc) dlsym(handle, "pushBack");
    printListFunc printList = (printListFunc) dlsym(handle, "printList");
    editContactFunc editContact = (editContactFunc) dlsym(handle, "editContact");
    deletePersonFunc deletePerson = (deletePersonFunc) dlsym(handle, "deletePerson");
    deleteListFunc deleteList = (deleteListFunc) dlsym(handle, "deleteList");
    insertionSortFunc insertionSort = (insertionSortFunc) dlsym(handle, "insertionSort");

    // Check if function loading was successful
    if (!createPerson || !addPerson || !printList || !editContact || !deletePerson || !deleteList || !insertionSort) {
        fprintf(stderr, "Error loading functions from dynamic library: %s\n", dlerror());
        return 1;
    }

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

                // Add person to list
                addPerson(peopleList, newPerson);
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
                dlclose(handle);
                return 0;

            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
