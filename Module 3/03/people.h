#ifndef PEOPLE_H
#define PEOPLE_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


#define MAX_PEOPLE 128
#define MAX_CHAR_LENGHT 50

typedef struct {
    char workplace[MAX_CHAR_LENGHT];
    char post[MAX_CHAR_LENGHT];
} job;

typedef struct {
    char personalPhone[MAX_CHAR_LENGHT];
    char workPhone[MAX_CHAR_LENGHT];
} contacts;

typedef struct {
    int id;
    char name[MAX_CHAR_LENGHT];
    char surname[MAX_CHAR_LENGHT];
    char patronymic[MAX_CHAR_LENGHT];
    char email[MAX_CHAR_LENGHT];
    char socialMedia[MAX_CHAR_LENGHT];
    job occupation;
    contacts phoneNumber;
} person;

void ClearNewline(char *str);
void AddPerson(person human);
void PeopleList();
void EditPerson(char Surname[MAX_CHAR_LENGHT], char Name[MAX_CHAR_LENGHT]);
void DeletePerson(char Surname[MAX_CHAR_LENGHT], char Name[MAX_CHAR_LENGHT]);
void LoadFromFile();
void SaveToFile(); 

#endif
