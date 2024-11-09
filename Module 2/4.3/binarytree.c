#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

typedef struct Node {
    person data;
    struct Node *left;
    struct Node *right;
} Node;

Node* createNode(person human) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = human;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node* insert(Node* root, person human) {
    if (root == NULL) return createNode(human);
    if (strcmp(human.surname, root->data.surname) < 0) {
        root->left = insert(root->left, human);
    } else if (strcmp(human.surname, root->data.surname) > 0) {
        root->right = insert(root->right, human);
    }
    return root;
}

Node* findMin(Node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

Node* deleteNode(Node* root, char* surname) {
    if (root == NULL) return root;
    if (strcmp(surname, root->data.surname) < 0) {
        root->left = deleteNode(root->left, surname);
    } else if (strcmp(surname, root->data.surname) > 0) {
        root->right = deleteNode(root->right, surname);
    } else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        } else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }
        Node* temp = findMin(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data.surname);
    }
    return root;
}

void inOrder(Node* root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("Surname: %s, Name: %s\n", root->data.surname, root->data.name);
        inOrder(root->right);
    }
}

void ClearNewline(char *str) {
    if (strlen(str) > 0 && str[strlen(str) - 1] == '\n') {
        str[strlen(str) - 1] = '\0';
    }
}

void PeopleList(Node* root) {
    if (root != NULL) {
        PeopleList(root->left);
        printf("-----------------------------------\n");
        printf("Surname: %s\n", root->data.surname);
        printf("Name: %s\n", root->data.name);
        printf("Patronymic: %s\n", root->data.patronymic);
        printf("Email: %s\n", root->data.email);
        printf("Workplace: %s\n", root->data.occupation.workplace);
        printf("Post: %s\n", root->data.occupation.post);
        printf("Personal phone: %s\n", root->data.phoneNumber.personalPhone);
        printf("Work phone: %s\n", root->data.phoneNumber.workPhone);
        printf("Social media: %s\n", root->data.socialMedia);
        PeopleList(root->right);
    }
}

void EditPerson(Node* root, char Surname[MAX_CHAR_LENGHT], char Name[MAX_CHAR_LENGHT], person newData) {
    if (root == NULL) return;
    if (strcmp(root->data.surname, Surname) < 0) {
        EditPerson(root->right, Surname, Name, newData);
    } else if (strcmp(root->data.surname, Surname) > 0) {
        EditPerson(root->left, Surname, Name, newData);
    } else if (strcmp(root->data.name, Name) == 0) {
        root->data = newData;
        printf("Person updated successfully!\n");
    }
}

int main() {
    Node* root = NULL;
    int choice;
    char Surname[MAX_CHAR_LENGHT], Name[MAX_CHAR_LENGHT];
    person human, updatedHuman;
    do {
        printf("Choose an action:\n1. Add new person\n2. People list\n3. Edit person\n4. Delete person\n5. Exit\n");
        scanf("%d", &choice);
        getchar();
        switch(choice) {
            case 1:
                printf("Enter surname: ");
                fgets(human.surname, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.surname);
                printf("Enter name: ");
                fgets(human.name, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.name);
                printf("Enter patronymic: ");
                fgets(human.patronymic, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.patronymic);
                printf("Enter email: ");
                fgets(human.email, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.email);
                printf("Enter workplace: ");
                fgets(human.occupation.workplace, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.occupation.workplace);
                printf("Enter post: ");
                fgets(human.occupation.post, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.occupation.post);
                printf("Enter personal phone number: ");
                fgets(human.phoneNumber.personalPhone, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.phoneNumber.personalPhone);
                printf("Enter work phone number: ");
                fgets(human.phoneNumber.workPhone, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.phoneNumber.workPhone);
                printf("Enter link to account in social media: ");
                fgets(human.socialMedia, MAX_CHAR_LENGHT, stdin);
                ClearNewline(human.socialMedia);
                root = insert(root, human);
                printf("Person added successfully!\n");
                break;
            case 2:
                PeopleList(root);
                break;
            case 3:
                printf("Enter Surname and Name to edit:\n");
                scanf("%s %s", Surname, Name);
                getchar();
                printf("Enter updated details\n");
                printf("Enter updated surname: ");
                fgets(updatedHuman.surname, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.surname);
                printf("Enter updated name: ");
                fgets(updatedHuman.name, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.name);
                printf("Enter updated patronymic: ");
                fgets(updatedHuman.patronymic, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.patronymic);
                printf("Enter updated email: ");
                fgets(updatedHuman.email, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.email);
                printf("Enter updated workplace: ");
                fgets(updatedHuman.occupation.workplace, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.occupation.workplace);
                printf("Enter updated post: ");
                fgets(updatedHuman.occupation.post, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.occupation.post);
                printf("Enter updated personal phone number: ");
                fgets(updatedHuman.phoneNumber.personalPhone, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.phoneNumber.personalPhone);
                printf("Enter updated work phone number: ");
                fgets(updatedHuman.phoneNumber.workPhone, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.phoneNumber.workPhone);
                printf("Enter updated social media link: ");
                fgets(updatedHuman.socialMedia, MAX_CHAR_LENGHT, stdin);
                ClearNewline(updatedHuman.socialMedia);
                EditPerson(root, Surname, Name, updatedHuman);
                break;
            case 4:
                printf("Enter Surname to delete:\n");
                scanf("%s", Surname);
                getchar();
                root = deleteNode(root, Surname);
                printf("Person deleted successfully!\n");
                break;
            case 5:
                printf("Exiting...\n");
                break;
        }
    } while (choice != 5);
    return 0;
}
