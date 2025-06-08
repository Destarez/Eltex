#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <sys/time.h>
#include <errno.h>

#define BUF_SIZE 1024
#define EXIT_MSG "__EXIT__"
#define MAX_RECV_ATTEMPTS 2

int client_up = 1;
int sockfd;
struct sockaddr_in client_addr, server_addr;
unsigned short client_port, server_port;

unsigned short ip_check_sum(unsigned short *buf, int len) {
    unsigned long sum = 0;
    while (len > 1) {
        sum += *buf++;
        len -= 2;
    }
    if (len > 0) {
        sum += *(unsigned char *)buf;
    }
    while (sum >> 16) {
        sum = (sum & 0xffff) + (sum >> 16);
    }
    return (unsigned short)(~sum);
}

unsigned short udp_check_sum(struct iphdr *iph, struct udphdr *udph, char *data, int data_len) {
    struct pseudohdr {
        unsigned int saddr;
        unsigned int daddr;
        unsigned char zero;
        unsigned char protocol;
        unsigned short length;
    } ph;

    ph.saddr = iph->saddr;
    ph.daddr = iph->daddr;
    ph.zero = 0;
    ph.protocol = IPPROTO_UDP;
    ph.length = udph->len;

    int total_len = sizeof(ph) + sizeof(struct udphdr) + data_len;
    char *buf = malloc(total_len);
    memcpy(buf, &ph, sizeof(ph));
    memcpy(buf + sizeof(ph), udph, sizeof(struct udphdr));
    memcpy(buf + sizeof(ph) + sizeof(struct udphdr), data, data_len);

    unsigned short checksum = ip_check_sum((unsigned short *)buf, total_len);
    free(buf);
    return checksum;
}

int port_pton(char* c_port, uint16_t* port) {
    char* endptr;
    uint16_t p = strtol(c_port, &endptr, 10);
    if (*endptr != '\0' || p <= 0 || p > 65535) return -1;
    *port = htons(p);
    return 0;
}

int setup_socket(char* cclient_ip, char* cclient_port, char* cserver_ip, char* cserver_port) {
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, cclient_ip, &client_addr.sin_addr) != 1) {
        perror("inet_pton client");
        return -1;
    }
    if (port_pton(cclient_port, &client_port) == -1) {
        perror("port_pton client");
        return -1;
    }
    client_addr.sin_port = client_port;

    if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("bind");
        return -1;
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, cserver_ip, &server_addr.sin_addr) != 1) {
        perror("inet_pton server");
        return -1;
    }
    if (port_pton(cserver_port, &server_port) == -1) {
        perror("port_pton server");
        return -1;
    }
    server_addr.sin_port = server_port;
    return 0;
}

void handle_signal(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        client_up = 0;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Usage: %s <client IP> <client port> <server IP> <server port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int on = 1;
    if (setsockopt(sockfd, IPPROTO_IP, IP_HDRINCL, &on, sizeof(on)) < 0) {
        perror("setsockopt");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
        perror("setsockopt timeout");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    if (setup_socket(argv[1], argv[2], argv[3], argv[4]) == -1) {
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("Connecting to server %s:%d from port %d\n", argv[3], ntohs(server_port), ntohs(client_port));
    printf("Client connected.\n");

    char buf[BUF_SIZE];
    struct sockaddr_in saddr;
    socklen_t saddr_size = sizeof(saddr);

    while (client_up) {
        printf("> ");
        fgets(buf, BUF_SIZE, stdin);
        buf[strcspn(buf, "\n")] = 0;

        if (strlen(buf) == 0) continue;

        if (strcmp(buf, EXIT_MSG) == 0) {
            client_up = 0;
            continue;
        }

        char packet[BUF_SIZE] = {0};
        struct iphdr* client_iph = (struct iphdr*)packet;
        client_iph->version = 4;
        client_iph->ihl = 5;
        client_iph->tos = 0;
        client_iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(buf) + 1);
        client_iph->id = htons(rand() % 65536);
        client_iph->frag_off = 0;
        client_iph->ttl = 64;
        client_iph->protocol = IPPROTO_UDP;
        client_iph->saddr = client_addr.sin_addr.s_addr;
        client_iph->daddr = server_addr.sin_addr.s_addr;
        client_iph->check = ip_check_sum((unsigned short *)client_iph, client_iph->ihl * 4);

        struct udphdr* client_udph = (struct udphdr*)(packet + sizeof(struct iphdr));
        client_udph->source = client_port;
        client_udph->dest = server_port;
        client_udph->len = htons(sizeof(struct udphdr) + strlen(buf) + 1);
        client_udph->check = 0;

        char* data = packet + sizeof(struct iphdr) + sizeof(struct udphdr);
        memcpy(data, buf, strlen(buf) + 1);
        client_udph->check = udp_check_sum(client_iph, client_udph, data, strlen(buf) + 1);

        printf("Sending to %s:%d\n", inet_ntoa(server_addr.sin_addr), ntohs(server_addr.sin_port));
        ssize_t sent = sendto(sockfd, packet, ntohs(client_iph->tot_len), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
        if (sent < 0) {
            fprintf(stderr, "sendto failed: %s\n", strerror(errno));
            continue;
        } else {
            printf("Sent %zd bytes\n", sent);
        }

        if (!client_up) break;

        int attempts = 0;
        int valid_packet = 0;
        while (attempts < MAX_RECV_ATTEMPTS && !valid_packet && client_up) {
            memset(buf, 0, BUF_SIZE);
            ssize_t recv_len = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr*)&saddr, &saddr_size);
            if (recv_len < 0) {
                fprintf(stderr, "recvfrom failed: %s\n", strerror(errno));
                break;
            }

            struct iphdr* reply_iph = (struct iphdr*)buf;
            unsigned int iphlen = reply_iph->ihl * 4;
            if (iphlen + sizeof(struct udphdr) > (unsigned int)recv_len) {
                attempts++;
                continue;
            }
            struct udphdr* reply_udph = (struct udphdr*)(buf + iphlen);
            if (reply_iph->protocol != IPPROTO_UDP ||
                ntohs(reply_udph->dest) != ntohs(client_port) ||
                reply_iph->saddr != server_addr.sin_addr.s_addr ||
                ntohs(reply_udph->source) != ntohs(server_port)) {
                attempts++;
                continue;
            }

            char* reply_data = buf + iphlen + sizeof(struct udphdr);
            int data_len = ntohs(reply_udph->len) - sizeof(struct udphdr);
            if (data_len < 0 || data_len >= BUF_SIZE) {
                attempts++;
                continue;
            }
            reply_data[data_len] = '\0';
            printf("< %s\n", reply_data);
            valid_packet = 1;
        }

        if (!valid_packet && client_up) {
            printf("No valid response received after %d attempts\n", MAX_RECV_ATTEMPTS);
        }
    }

    close(sockfd);
    printf("Client disconnected.\n");
    return 0;
}