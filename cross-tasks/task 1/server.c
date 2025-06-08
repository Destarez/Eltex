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
#include <errno.h>

#define BUF_SIZE 1024
#define MAX_CLIENTS 10
#define EXIT_MSG "__EXIT__"

typedef struct {
    struct sockaddr_in client_addr;
    socklen_t client_addr_len;
    int count;
} ClientInfo;

int server_up = 1;
int clients_count = 0;
ClientInfo clients[MAX_CLIENTS];
int sockfd;
struct sockaddr_in server_addr;
unsigned short server_port;

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

int find_client(ClientInfo* client_info) {
    for (int i = 0; i < clients_count; i++) {
        if (clients[i].client_addr.sin_addr.s_addr == client_info->client_addr.sin_addr.s_addr &&
            clients[i].client_addr.sin_port == client_info->client_addr.sin_port) {
            return i;
        }
    }
    return -1;
}

int add_client(ClientInfo* client_info) {
    if (clients_count >= MAX_CLIENTS) return -1;
    clients[clients_count].client_addr = client_info->client_addr;
    clients[clients_count].client_addr_len = sizeof(struct sockaddr_in);
    clients[clients_count].count = 1;
    clients_count++;
    return 0;
}

int reset_client(ClientInfo* client_info) {
    int idx = find_client(client_info);
    if (idx == -1) return -1;
    clients[idx].count++;
    client_info->count = clients[idx].count;
    return 0;
}

int clear_client(ClientInfo* client_info) {
    int idx = find_client(client_info);
    if (idx == -1) return -1;
    memset(&clients[idx], 0, sizeof(ClientInfo));
    return 0;
}

void handle_signal(int sig) {
    if (sig == SIGINT || sig == SIGTERM) {
        server_up = 0;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server IP> <server port>\n", argv[0]);
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

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) != 1) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    if (port_pton(argv[2], &server_addr.sin_port) == -1) {
        fprintf(stderr, "Invalid port: %s\n", argv[2]);
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    server_port = ntohs(server_addr.sin_port);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    printf("Server listening on %s:%d\n", argv[1], server_port);

    char buf[BUF_SIZE];
    struct sockaddr_in saddr;
    socklen_t saddr_size;

    while (server_up) {
        saddr_size = sizeof(saddr);
        ssize_t receive_size = recvfrom(sockfd, buf, BUF_SIZE, 0, (struct sockaddr*)&saddr, &saddr_size);
        if (receive_size < 0) {
            fprintf(stderr, "recvfrom failed: %s\n", strerror(errno));
            continue;
        }

        struct iphdr* iph = (struct iphdr*)buf;
        unsigned int iphlen = iph->ihl * 4;
        if (iphlen + sizeof(struct udphdr) > (unsigned int)receive_size) {
            continue;
        }
        struct udphdr* udph = (struct udphdr*)(buf + iphlen);
        if (iph->protocol != IPPROTO_UDP || ntohs(udph->dest) != server_port) {
            continue;
        }

        char* client_data = buf + iphlen + sizeof(struct udphdr);
        int data_len = ntohs(udph->len) - sizeof(struct udphdr);
        if (data_len < 0 || data_len >= BUF_SIZE) {
            continue;
        }
        client_data[data_len] = '\0';

        ClientInfo client_info = { .client_addr = saddr, .client_addr_len = saddr_size, .count = 1 };
        client_info.client_addr.sin_port = udph->source;

        if (strcmp(client_data, EXIT_MSG) == 0) {
            if (clear_client(&client_info) != -1) {
                printf("Client %s:%d disconnected\n", inet_ntoa(client_info.client_addr.sin_addr), ntohs(client_info.client_addr.sin_port));
            }
            continue;
        }

        if (reset_client(&client_info) == -1) {
            if (add_client(&client_info) != -1) {
                printf("Client %s:%d connected\n", inet_ntoa(client_info.client_addr.sin_addr), ntohs(client_info.client_addr.sin_port));
            } else {
                printf("Client limit reached\n");
                continue;
            }
        }

        char reply[BUF_SIZE];
        snprintf(reply, BUF_SIZE, "%s %d", client_data, client_info.count);
        printf("Client %s:%d sent: %s\n", inet_ntoa(client_info.client_addr.sin_addr), ntohs(client_info.client_addr.sin_port), client_data);
        printf("Server sending: %s\n", reply);

        char packet[BUF_SIZE] = {0};
        struct iphdr* server_iph = (struct iphdr*)packet;
        server_iph->version = 4;
        server_iph->ihl = 5;
        server_iph->tos = 0;
        server_iph->tot_len = htons(sizeof(struct iphdr) + sizeof(struct udphdr) + strlen(reply) + 1);
        server_iph->id = htons(rand() % 65536);
        server_iph->frag_off = 0;
        server_iph->ttl = 64;
        server_iph->protocol = IPPROTO_UDP;
        server_iph->saddr = iph->daddr;
        server_iph->daddr = iph->saddr;
        server_iph->check = ip_check_sum((unsigned short *)server_iph, server_iph->ihl * 4);

        struct udphdr* server_udph = (struct udphdr*)(packet + sizeof(struct iphdr));
        server_udph->source = udph->dest;
        server_udph->dest = udph->source;
        server_udph->len = htons(sizeof(struct udphdr) + strlen(reply) + 1);
        server_udph->check = 0;

        memcpy(packet + sizeof(struct iphdr) + sizeof(struct udphdr), reply, strlen(reply) + 1);
        server_udph->check = udp_check_sum(server_iph, server_udph, reply, strlen(reply) + 1);

        ssize_t sent = sendto(sockfd, packet, ntohs(server_iph->tot_len), 0, (struct sockaddr*)&client_info.client_addr, saddr_size);
        if (sent < 0) {
            fprintf(stderr, "sendto failed: %s\n", strerror(errno));
        }
    }

    close(sockfd);
    printf("Server closed\n");
    return 0;
}