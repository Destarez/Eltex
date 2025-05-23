#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <stdlib.h>
#define MAX_LENGHT 20

void mode_to_string(mode_t mode, char *str) {
    strcpy(str, "----------");
    if (S_ISDIR(mode)) str[0] = 'd';
    if (mode & S_IRUSR) str[1] = 'r';
    if (mode & S_IWUSR) str[2] = 'w';
    if (mode & S_IXUSR) str[3] = 'x';
    if (mode & S_IRGRP) str[4] = 'r';
    if (mode & S_IWGRP) str[5] = 'w';
    if (mode & S_IXGRP) str[6] = 'x';
    if (mode & S_IROTH) str[7] = 'r';
    if (mode & S_IWOTH) str[8] = 'w';
    if (mode & S_IXOTH) str[9] = 'x';
}

void print_bit_representation(mode_t mode) {
    for (int i = 8; i >= 0; i--) {
        printf("%d", (mode >> i) & 1);
    }
    printf("\n");
}

void Touch(char *filename) {
    int fd = open(filename, O_WRONLY | O_CREAT, 0644);
    if(fd == -1){
        perror("Cant create file.");
        return;
    }
    close(fd);
}

void Ls(char *arg1){
    DIR *dir = opendir(".");
    if (!dir) {
        perror("opendir");
        return;
    }

    struct dirent *entry;
    struct stat file_info;

    if (arg1 && strcmp(arg1, "-l") == 0) {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] == '.') continue;

            if (stat(entry->d_name, &file_info) == -1) {
                perror(entry->d_name);
                continue;
            }

            char mode_str[11];
            mode_to_string(file_info.st_mode, mode_str);
            printf("%s ", mode_str);

            printf("%6ld ", file_info.st_size);

            char time_buf[20];
            struct tm *time_info = localtime(&file_info.st_mtime);
            strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M", time_info);
            printf("%s ", time_buf);

            printf("%s\n", entry->d_name);

            printf("Bitwise: ");
            print_bit_representation(file_info.st_mode & 0777);
        }
    } else {
        while ((entry = readdir(dir)) != NULL) {
            if (entry->d_name[0] != '.') {
                printf("%s\n", entry->d_name);
            }
        }
    }

    closedir(dir);
}


void CHMOD(char *mode_str, char *filename) {
    if (mode_str == NULL || filename == NULL) {
        printf("Usage: chmod <mode> <filename>\n");
        return;
    }

    mode_t mode = strtol(mode_str, NULL, 8);
    if (chmod(filename, mode) == -1) {
        perror("chmod");
    } else {
        printf("Permissions of '%s' changed to %s\n", filename, mode_str);
    }
}

int main() {
    char input[100];

    while (1) {
        printf(">>> ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = '\0';

        char *command = strtok(input, " ");
        char *arg1 = strtok(NULL, " ");
        char *arg2 = strtok(NULL, " ");

        if (!command) continue;

        if (strcmp(command, "exit") == 0) {
            break;
        } else if (strcmp(command, "touch") == 0) {
            if (arg1) Touch(arg1);
            else printf("Usage: touch <filename>\n");
        } else if (strcmp(command, "ls") == 0) {
            Ls(arg1);
        } else if (strcmp(command, "chmod") == 0) {
            CHMOD(arg1, arg2);
        } else {
            printf("Unknown command: %s\n", command);
        }
    }

    return 0;
}