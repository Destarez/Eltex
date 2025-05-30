#include <arpa/inet.h>
#include <fcntl.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE (sizeof(struct iphdr) + sizeof(struct udphdr) + 1000)

void handlePacket(const char* buffer, int size){
    struct iphdr* iph = (struct iphdr*)buffer;
    int iphdr_len = iph->ihl * 4;

    if (size < iphdr_len + sizeof(struct udphdr) + sizeof(int)) {
        printf("Packet too short\n");
        return;
    }

    struct udphdr* udph = (struct udphdr*)(buffer + iphdr_len);

    if(iph->saddr != htonl(0x7f000001) || udph->source != htons(8080)){
        return;
    }

    printf("\nReceived packet: \n");
    struct in_addr src, dst;
    src.s_addr = iph->saddr;
    dst.s_addr = iph->daddr;
    printf("Source IP: %s\n", inet_ntoa(src));
    printf("Destination IP: %s\n", inet_ntoa(dst));
    printf("Source Port: %d\n", ntohs(udph->source));
    printf("Destination Port: %d\n", ntohs(udph->dest));

    int offset = iphdr_len + sizeof(struct udphdr) + sizeof(int);
    int payload_len = size - offset;
    const char* data = buffer + offset;

    printf("Data Size: %d bytes\n", payload_len);

    if (payload_len > 0) {
        printf("Data: %.*s\n", payload_len, data);
        int fd = open("file.bin", O_CREAT | O_WRONLY | O_APPEND, 0777);
        if (fd == -1) {
            perror("open");
            return;
        }
        if (write(fd, data, payload_len) == -1) {
            perror("write");
        }
        close(fd);
    } else {
        printf("No data in packet\n");
    }
}

int main(){
    int sockfd;
    char buffer[BUF_SIZE];
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if((sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) < 0){
        perror("socket create");
        exit(EXIT_FAILURE);
    }

    while(1){
        int dataSize = recvfrom(sockfd, buffer, BUF_SIZE, 0, (struct sockaddr*)&addr, &addr_len);
        if(dataSize <= 0) {
            perror("recv failed");
            break;
        }

        handlePacket(buffer, dataSize);
        memset(buffer, 0, BUF_SIZE);
    }

    close(sockfd);
    return 0;
}
