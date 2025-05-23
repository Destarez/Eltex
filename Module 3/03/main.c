#include "people.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    LoadFromFile();
    int choice;
    do {
        printf("-----------------------------------\n");
        printf("Choose an action:\n");
        printf("1. Add new person\n");
        printf("2. People list\n");
        printf("3. Edit person\n");
        printf("4. Delete person\n");
        printf("5. Exit\n");
        printf("-----------------------------------\n");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                person human;
                char input[MAX_CHAR_LENGHT];

                printf("Enter surname: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.surname, input);

                printf("Enter name: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.name, input);

                printf("Enter patronymic: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.patronymic, input);

                printf("Enter email: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.email, input);

                printf("Enter workplace: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.occupation.workplace, input);

                printf("Enter post: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.occupation.post, input);

                printf("Enter personal phone number: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.phoneNumber.personalPhone, input);

                printf("Enter work phone number: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.phoneNumber.workPhone, input);

                printf("Enter link to account in social media: ");
                fgets(input, MAX_CHAR_LENGHT, stdin);
                ClearNewline(input);
                strcpy(human.socialMedia, input);

                AddPerson(human);
                break;
            }
            case 2:
                PeopleList();
                break;
            case 3: {
                char Name[MAX_CHAR_LENGHT], Surname[MAX_CHAR_LENGHT];
                printf("Enter Surname and Name: ");
                scanf("%s %s", Surname, Name);
                getchar();
                EditPerson(Surname, Name);
                break;
            }
            case 4: {
                char Name[MAX_CHAR_LENGHT], Surname[MAX_CHAR_LENGHT];
                printf("Enter Surname and Name: ");
                scanf("%s %s", Surname, Name);
                getchar();
                DeletePerson(Surname, Name);
                break;
            }
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);

    SaveToFile();
    return 0;
}
