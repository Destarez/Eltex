#include <stdio.h>
#include <string.h>

#define MAX_PEOPLE 128
#define MAX_CHAR_LENGHT 50

typedef struct
{
    char workplace[MAX_CHAR_LENGHT];
    char post[MAX_CHAR_LENGHT];
}job;

typedef struct
{
    char personalPhone[MAX_CHAR_LENGHT];
    char workPhone[MAX_CHAR_LENGHT];
}contacts;

typedef struct 
{
    int id;
    char name[MAX_CHAR_LENGHT];
    char surname[MAX_CHAR_LENGHT];
    char patronymic[MAX_CHAR_LENGHT];
    char email[MAX_CHAR_LENGHT];
    char socialMedia[MAX_CHAR_LENGHT];
    job occupation;
    contacts phoneNumber;
}person;

void ClearNewline(char *str) {
    if (strlen(str) > 0 && str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

person people[MAX_PEOPLE];
int peopleCount=0;

void AddPerson()
{
    if (peopleCount >= MAX_PEOPLE)
    {
        printf("Phonebook is full!\n");
        return;
    }

    people[peopleCount].id = peopleCount + 1;

    printf("-----------------------------------\n");

    printf("Enter surname: ");
    fgets(people[peopleCount].surname, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].surname);

    printf("Enter name: ");
    fgets(people[peopleCount].name, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].name);


    printf("Enter patronymic: ");
    fgets(people[peopleCount].patronymic, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].patronymic);

    printf("Enter email: ");
    fgets(people[peopleCount].email, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].email);

    printf("Enter workplace: ");
    fgets(people[peopleCount].occupation.workplace, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].occupation.workplace);

    printf("Enter post: ");
    fgets(people[peopleCount].occupation.post, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].occupation.post);

    printf("Enter personal phone number: ");
    fgets(people[peopleCount].phoneNumber.personalPhone, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].phoneNumber.personalPhone);

    printf("Enter work phone number: ");
    fgets(people[peopleCount].phoneNumber.workPhone, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].phoneNumber.workPhone);

    printf("Enter link to account in social media: ");
    fgets(people[peopleCount].socialMedia, MAX_CHAR_LENGHT, stdin);
    ClearNewline(people[peopleCount].socialMedia);

    peopleCount++;

    printf("-----------------------------------\n");
    printf("Person added successfully!\n");
}

void PeopleList()
{
    for(int i=0;i<peopleCount;i++)
    {
        printf("-----------------------------------\n");
        printf("Contact №%d\n",i+1);
        printf("Surname: %s\n",people[i].surname);
        printf("Name: %s\n",people[i].name);
        printf("Patronymic: %s\n",people[i].patronymic);
        printf("Email: %s\n",people[i].email);
        printf("Workplace: %s\n",people[i].occupation.workplace);
        printf("Post: %s\n",people[i].occupation.post);
        printf("Personal phone: %s\n",people[i].phoneNumber.personalPhone);
        printf("Work phone: %s\n",people[i].phoneNumber.workPhone);
        printf("Social media: %s\n",people[i].socialMedia);

    }
}

void EditPerson()
{
    char Name[MAX_CHAR_LENGHT], Surname[MAX_CHAR_LENGHT];
    printf("-----------------------------------\n");
    printf("Enter Surname and Name\n");
    scanf("%s %s", Surname, Name);
    printf("-----------------------------------\n");
    getchar();
    int found=0;

    for( int i=0;i<peopleCount;i++)
    {
        if ((strcmp(people[i].surname,Surname)==0)&& (strcmp(people[i].name,Name)==0))
        {   
            found=1;
            char input[MAX_CHAR_LENGHT];
            
            printf("Editing person %s %s...\n", Surname, Name);
            
            printf("Enter patronymic: ");

            fgets(input,MAX_CHAR_LENGHT,stdin);
            ClearNewline(input);
            if (input[0]!='\0')
            {
                strncpy(people[i].patronymic, input, MAX_CHAR_LENGHT);
                ClearNewline(people[i].patronymic);
            }

            printf("Enter email: ");

            fgets(input,MAX_CHAR_LENGHT,stdin);
            ClearNewline(input);
            if (input[0]!='\0')
            {
                strncpy(people[i].email, input, MAX_CHAR_LENGHT);
                ClearNewline(people[i].email);
            }
            
            printf("Enter workplace: ");

            fgets(input,MAX_CHAR_LENGHT,stdin);
            ClearNewline(input);
            if (input[0]!='\0')
            {
                strncpy(people[i].occupation.workplace, input, MAX_CHAR_LENGHT);
                ClearNewline(people[i].occupation.workplace);
            }

            printf("Enter post: ");

            fgets(input,MAX_CHAR_LENGHT,stdin);
            ClearNewline(input);
            if (input[0]!='\0')
            {
                strncpy(people[i].occupation.post, input, MAX_CHAR_LENGHT);
                ClearNewline(people[i].occupation.post);
            }

            printf("Enter personal phone: ");

            fgets(input,MAX_CHAR_LENGHT,stdin);
            ClearNewline(input);
            if (input[0]!='\0')
            {
                strncpy(people[i].phoneNumber.personalPhone, input, MAX_CHAR_LENGHT);
                ClearNewline(people[i].phoneNumber.personalPhone);
            }

            printf("Enter work phone: ");

            fgets(input,MAX_CHAR_LENGHT,stdin);
            ClearNewline(input);
            if (input[0]!='\0')
            {
                strncpy(people[i].phoneNumber.workPhone, input, MAX_CHAR_LENGHT);
                ClearNewline(people[i].phoneNumber.workPhone);
            }

            printf("Enter social media: ");

            fgets(input,MAX_CHAR_LENGHT,stdin);
            ClearNewline(input);
            if (input[0]!='\0')
            {
                strncpy(people[i].socialMedia, input, MAX_CHAR_LENGHT);
                ClearNewline(people[i].socialMedia);
            }
        }
    }
    if (found==0) printf("Person not found\n");
}

void DeletePerson()
{
    char Name[MAX_CHAR_LENGHT], Surname[MAX_CHAR_LENGHT];
    printf("Enter Surname and Name\n");
    scanf("%s %s", Surname, Name);
    printf("-----------------------------------\n");
    getchar();
    int found=0;

    for( int i=0;i<peopleCount;i++)
    {
        if ((strcmp(people[i].surname,Surname)==0)&& (strcmp(people[i].name,Name)==0))
        {   
            found=1;
            strcpy(people[i].surname,"");
            strcpy(people[i].name,"");
            strcpy(people[i].patronymic,"");
            strcpy(people[i].email,"");
            strcpy(people[i].occupation.workplace,"");
            strcpy(people[i].occupation.post,"");
            strcpy(people[i].phoneNumber.personalPhone,"");
            strcpy(people[i].phoneNumber.workPhone,"");
            strcpy(people[i].socialMedia,"");

            printf("Deleting person %s %s...\n", Surname, Name);
            printf("-----------------------------------\n");

            for (int j=i; j<peopleCount - 1; j++)
            {
                people[j] = people [j+1];
            }
            peopleCount--;
            printf("-----------------------------------\n");
            printf("Person deleted\n");
        }
    }
    if(found==0) printf("Person not found\n");
}

int main()
{
    int choice,id;
    do
    {   
        printf("-----------------------------------\n");
        printf("Choose an action:\n");
        printf("1. Add new person:\n");
        printf("2. People list:\n");
        printf("3. Edit person:\n");
        printf("4. Delete person:\n");
        printf("5. Exit:\n");
        printf("-----------------------------------\n");
        scanf("%d",&choice);
        getchar();
        switch(choice)
        {
            case 1:
                AddPerson();
                break;
            case 2:
                PeopleList();
                break;
            case 3:
                EditPerson();
                break;
            case 4:
                DeletePerson();
                break;
            case 5:
                printf("-----------------------------------\nExiting...\n-----------------------------------\n");
                break;
        }
    } 
    while (choice!=5);
    return 0;
    
}