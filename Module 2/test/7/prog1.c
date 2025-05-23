#include <fcntl.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


int main(){
    mqd_t mq;
    struct mq_attr attr = {
        .mq_flags = 0,
        .mq_maxmsg = 10,
        .mq_msgsize = 1024,
        .mq_curmsgs = 0
    };

    mq = mq_open("/chat_queue", O_CREAT | O_RDWR, 0644, &attr);
    if (mq == (mqd_t)-1) {
        perror("mq_open");
        return 1;
    }

    char buffer[1024];
    unsigned int prio;

    printf("User1: ");
    fgets(buffer, sizeof(buffer), stdin);
    if (mq_send(mq, buffer, strlen(buffer) + 1, 1) == -1) {
        perror("mq_send");
    }

    while (1) {
        // Получаем ответ от user2
        ssize_t bytes_read = mq_receive(mq, buffer, sizeof(buffer), &prio);
        if (bytes_read == -1) {
            perror("mq_receive");
            break;
        }

        // Проверяем, не завершающее ли это сообщение
        if (prio == 255) {
            printf("User2 ended chat.\n");
            break;
        }

        printf("User2: %s", buffer);

        // Отправляем новое сообщение
        printf("User1: ");
        fgets(buffer, sizeof(buffer), stdin);
        if (mq_send(mq, buffer, strlen(buffer) + 1, 1) == -1) {
            perror("mq_send");
            break;
        }
    }
    mq_close(mq);
    mq_unlink("/chat_queue"); // Удаляем очередь (может сделать только один процесс)
    return 0;
}
