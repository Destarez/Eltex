#include "people.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#define FILE_PATH "data.bin"



person people[MAX_PEOPLE];
int peopleCount = 0;

void ClearNewline(char *str) {
    if (strlen(str) > 0 && str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

void AddPerson(person human) {
    if (peopleCount >= MAX_PEOPLE) {
        printf("Phonebook is full!\n");
        return;
    }
    
    human.id = peopleCount + 1;
    people[peopleCount] = human;
    peopleCount++;

    printf("-----------------------------------\n");
    printf("Person added successfully!\n");
}

void PeopleList() {
    for (int i = 0; i < peopleCount; i++) {
        printf("-----------------------------------\n");
        printf("Contact №%d\n", i + 1);
        printf("Surname: %s\n", people[i].surname);
        printf("Name: %s\n", people[i].name);
        printf("Patronymic: %s\n", people[i].patronymic);
        printf("Email: %s\n", people[i].email);
        printf("Workplace: %s\n", people[i].occupation.workplace);
        printf("Post: %s\n", people[i].occupation.post);
        printf("Personal phone: %s\n", people[i].phoneNumber.personalPhone);
        printf("Work phone: %s\n", people[i].phoneNumber.workPhone);
        printf("Social media: %s\n", people[i].socialMedia);
    }
}

void EditPerson(char Surname[MAX_CHAR_LENGHT], char Name[MAX_CHAR_LENGHT]) {
    int found = 0;

    for (int i = 0; i < peopleCount; i++) {
        if ((strcmp(people[i].surname, Surname) == 0) && (strcmp(people[i].name, Name) == 0)) {
            found = 1;
            char input[MAX_CHAR_LENGHT];

            printf("Editing person %s %s...\n", Surname, Name);

            printf("Enter patronymic: ");
            fgets(input, MAX_CHAR_LENGHT, stdin);
            ClearNewline(input);
            if (input[0] != '\0') {
                strncpy(people[i].patronymic, input, MAX_CHAR_LENGHT);
            }

            printf("Enter email: ");
            fgets(input, MAX_CHAR_LENGHT, stdin);
            ClearNewline(input);
            if (input[0] != '\0') {
                strncpy(people[i].email, input, MAX_CHAR_LENGHT);
            }

            printf("Enter workplace: ");
            fgets(input, MAX_CHAR_LENGHT, stdin);
            ClearNewline(input);
            if (input[0] != '\0') {
                strncpy(people[i].occupation.workplace, input, MAX_CHAR_LENGHT);
            }

            printf("Enter post: ");
            fgets(input, MAX_CHAR_LENGHT, stdin);
            ClearNewline(input);
            if (input[0] != '\0') {
                strncpy(people[i].occupation.post, input, MAX_CHAR_LENGHT);
            }

            printf("Enter personal phone: ");
            fgets(input, MAX_CHAR_LENGHT, stdin);
            ClearNewline(input);
            if (input[0] != '\0') {
                strncpy(people[i].phoneNumber.personalPhone, input, MAX_CHAR_LENGHT);
            }

            printf("Enter work phone: ");
            fgets(input, MAX_CHAR_LENGHT, stdin);
            ClearNewline(input);
            if (input[0] != '\0') {
                strncpy(people[i].phoneNumber.workPhone, input, MAX_CHAR_LENGHT);
            }

            printf("Enter social media: ");
            fgets(input, MAX_CHAR_LENGHT, stdin);
            ClearNewline(input);
            if (input[0] != '\0') {
                strncpy(people[i].socialMedia, input, MAX_CHAR_LENGHT);
            }
        }
    }
    if (found == 0) printf("Person not found\n");
}

void DeletePerson(char Surname[MAX_CHAR_LENGHT], char Name[MAX_CHAR_LENGHT]) {
    int found = 0;

    for (int i = 0; i < peopleCount; i++) {
        if ((strcmp(people[i].surname, Surname) == 0) && (strcmp(people[i].name, Name) == 0)) {
            found = 1;
            for (int j = i; j < peopleCount - 1; j++) {
                people[j] = people[j + 1];
            }
            peopleCount--;
            printf("Person deleted\n");
            break;
        }
    }
    if (found == 0) printf("Person not found\n");
}

void LoadFromFile()
{
    int fd = open(FILE_PATH, O_RDONLY);
    if (fd < 0) return;

    ssize_t bytes = read(fd,people, sizeof(person) * MAX_PEOPLE);
    if (bytes > 0)
    {
        peopleCount = bytes / sizeof(person);
    }

    close(fd);
}


void SaveToFile() {
    int fd = open(FILE_PATH, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Unable to save file");
        return;
    }

    write(fd, people, sizeof(person) * peopleCount);
    close(fd);
}