#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

#define MAX_DRIVERS 100
#define FIFO_PATH "taxi_%d"
#define MAX_COMMAND 256

typedef struct {
    pid_t pid;
    int fifo_fd;
    char fifo_name[50];
} Driver;

Driver drivers[MAX_DRIVERS];
int driver_count = 0;

void create_driver() {
    if (driver_count >= MAX_DRIVERS) {
        printf("Maximum drivers reached\n");
        return;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork");
        return;
    } else if (pid == 0) {
        char pid_str[16];
        snprintf(pid_str, sizeof(pid_str), "%d", getpid());
        execl("./driver", "./driver", pid_str, NULL);
        perror("execl");
        exit(1);
    } else {
        char fifo_name[50];
        snprintf(fifo_name, sizeof(fifo_name), FIFO_PATH, pid);
        
        int fifo_fd = -1;
        for (int i = 0; i < 10 && fifo_fd < 0; i++) {
            fifo_fd = open(fifo_name, O_WRONLY | O_NONBLOCK);
            if (fifo_fd < 0) usleep(100000);
        }
        
        if (fifo_fd < 0) {
            printf("Failed to open FIFO for driver %d\n", pid);
            kill(pid, SIGTERM);
            return;
        }

        drivers[driver_count].pid = pid;
        drivers[driver_count].fifo_fd = fifo_fd;
        strcpy(drivers[driver_count].fifo_name, fifo_name);
        driver_count++;
        printf("Driver created with PID %d\n", pid);
    }
}

void send_task(pid_t pid, int task_timer) {
    for (int i = 0; i < driver_count; i++) {
        if (drivers[i].pid == pid) {
            char command[MAX_COMMAND];
            snprintf(command, sizeof(command), "TASK %d", task_timer);
            if (write(drivers[i].fifo_fd, command, strlen(command) + 1) < 0) {
                perror("Failed to send task");
            }
            return;
        }
    }
    printf("Driver %d not found\n", pid);
}

void get_status(pid_t pid) {
    for (int i = 0; i < driver_count; i++) {
        if (drivers[i].pid == pid) {
            char command[] = "STATUS";
            if (write(drivers[i].fifo_fd, command, strlen(command) + 1) < 0) {
                perror("get status");
            }
            return;
        }
    }
    printf("Driver %d not found\n", pid);
}

void get_drivers() {
    if (driver_count == 0) {
        printf("No drivers available\n");
        return;
    }
    for (int i = 0; i < driver_count; i++) {
        printf("Driver PID: %d\n", drivers[i].pid);
        get_status(drivers[i].pid);
    }
}

void handle_response(int fd) {
    char buffer[256];
    ssize_t n = read(fd, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("%s\n", buffer);
    }
}

int main() {
    char command[256];
    fd_set read_fds;
    int max_fd = 0;

    char response_fifo[] = "/tmp/taxi_response";
    unlink(response_fifo);
    if (mkfifo(response_fifo, 0666) < 0) {
        perror("create response FIFO");
        exit(1);
    }

    int response_fd = open(response_fifo, O_RDONLY | O_NONBLOCK);
    if (response_fd < 0) {
        perror("open response FIFO");
        exit(1);
    }

    while (1) {
        printf("Enter command (create_driver, send_task <pid> <timer>, get_status <pid>, get_drivers, exit): ");
        if (!fgets(command, sizeof(command), stdin)) {
            continue;
        }
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "create_driver") == 0) {
            create_driver();
        } else if (strncmp(command, "send_task ", 10) == 0) {
            pid_t pid;
            int timer;
            if (sscanf(command, "send_task %d %d", &pid, &timer) == 2) {
                send_task(pid, timer);
            } else {
                printf("Invalid send_task format\n");
            }
        } else if (strncmp(command, "get_status ", 11) == 0) {
            pid_t pid;
            if (sscanf(command, "get_status %d", &pid) == 1) {
                get_status(pid);
            } else {
                printf("Invalid get_status format\n");
            }
        } else if (strcmp(command, "get_drivers") == 0) {
            get_drivers();
        } else if (strcmp(command, "exit") == 0) {
            break;
        } else {
            printf("Unknown command\n");
        }

        FD_ZERO(&read_fds);
        FD_SET(response_fd, &read_fds);
        max_fd = response_fd;

        struct timeval timeout = {1, 0};
        if (select(max_fd + 1, &read_fds, NULL, NULL, &timeout) > 0) {
            if (FD_ISSET(response_fd, &read_fds)) {
                handle_response(response_fd);
            }
        }
    }

    for (int i = 0; i < driver_count; i++) {
        close(drivers[i].fifo_fd);
        unlink(drivers[i].fifo_name);
        kill(drivers[i].pid, SIGTERM);
    }
    close(response_fd);
    unlink(response_fifo);
    return 0;
}