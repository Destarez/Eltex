#include "people.h"
#include <stdio.h>
#include <string.h>

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