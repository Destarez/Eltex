#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/timerfd.h>
#include <sys/select.h>
#include <signal.h>
#include <stdint.h>

#define FIFO_PATH "taxi_%d"
#define RESPONSE_FIFO "taxi_response"

int busy = 0;
int task_timer = 0;
int response_fd = -1;

void timer_handler(int fd) {
    uint64_t expirations;
    read(fd, &expirations, sizeof(expirations));
    busy = 0;
    task_timer = 0;
}

void send_response(const char *response) {
    if (response_fd < 0) {
        response_fd = open(RESPONSE_FIFO, O_WRONLY | O_NONBLOCK);
        if (response_fd < 0) return;
    }
    write(response_fd, response, strlen(response) + 1);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        exit(1);
    }

    pid_t pid = atoi(argv[1]);
    char fifo_name[50];
    snprintf(fifo_name, sizeof(fifo_name), FIFO_PATH, pid);

    unlink(fifo_name);
    if (mkfifo(fifo_name, 0666) < 0) {
        perror("create FIFO");
        exit(1);
    }

    int fifo_fd = open(fifo_name, O_RDONLY | O_NONBLOCK);
    if (fifo_fd < 0) {
        perror("open FIFO");
        unlink(fifo_name);
        exit(1);
    }

    int timer_fd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK);
    if (timer_fd < 0) {
        perror("create timer");
        close(fifo_fd);
        unlink(fifo_name);
        exit(1);
    }

    fd_set read_fds;
    int max_fd = (fifo_fd > timer_fd ? fifo_fd : timer_fd) + 1;
    char buffer[256];

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(fifo_fd, &read_fds);
        FD_SET(timer_fd, &read_fds);

        int ret = select(max_fd, &read_fds, NULL, NULL, NULL);
        if (ret < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(timer_fd, &read_fds)) {
            timer_handler(timer_fd);
        }

        if (FD_ISSET(fifo_fd, &read_fds)) {
            ssize_t n = read(fifo_fd, buffer, sizeof(buffer) - 1);
            if (n > 0) {
                buffer[n] = '\0';
                if (strcmp(buffer, "STATUS") == 0) {
                    char response[256];
                    if (busy) {
                        snprintf(response, sizeof(response), "Driver %d: Busy %d", pid, task_timer);
                    } else {
                        snprintf(response, sizeof(response), "Driver %d: Available", pid);
                    }
                    send_response(response);
                } else if (strncmp(buffer, "TASK ", 5) == 0) {
                    if (busy) {
                        char response[256];
                        snprintf(response, sizeof(response), "Driver %d: Busy %d", pid, task_timer);
                        send_response(response);
                    } else {
                        int timer;
                        if (sscanf(buffer, "TASK %d", &timer) == 1) {
                            busy = 1;
                            task_timer = timer;

                            struct itimerspec ts;
                            ts.it_value.tv_sec = timer;
                            ts.it_value.tv_nsec = 0;
                            ts.it_interval.tv_sec = 0;
                            ts.it_interval.tv_nsec = 0;
                            if (timerfd_settime(timer_fd, 0, &ts, NULL) < 0) {
                                perror("set timer");
                            }
                        }
                    }
                }
            }
        }
    }

    close(fifo_fd);
    close(timer_fd);
    unlink(fifo_name);
    if (response_fd >= 0) close(response_fd);
    return 0;
}