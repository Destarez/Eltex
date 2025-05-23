#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include "list.h"

int main() {
    double_linked_list contactList = {NULL, NULL};
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
                exit(1);
                break;

            default:
                printf("ERROR. Try again.\n");
        }
    } while (choice != 4);

    return 0;
}
