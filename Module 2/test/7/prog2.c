#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
    mqd_t mq;
    struct mq_attr attr;

    // Открываем очередь
    mq = mq_open("/chat_queue", O_RDWR);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    char buffer[1024];
    unsigned int prio;

    while (1) {
        // Ждем сообщение от user1
        ssize_t bytes_read = mq_receive(mq, buffer, sizeof(buffer), &prio);
        if (bytes_read == -1) {
            perror("mq_receive");
            break;
        }

        // Проверяем, не завершающее ли это сообщение
        if (prio == 255) {
            printf("User1 ended chat.\n");
            break;
        }

        printf("User1: %s", buffer);

        // Отправляем ответ
        printf("User2: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Если введено "exit", завершаем чат
        if (strcmp(buffer, "exit\n") == 0) {
            if (mq_send(mq, "Chat ended.", strlen("Chat ended.") + 1, 255) == -1) {
                perror("mq_send");
            }
            break;
        }

        if (mq_send(mq, buffer, strlen(buffer) + 1, 1) == -1) {
            perror("mq_send");
            break;
        }
    }

    mq_close(mq);
    return 0;
}
