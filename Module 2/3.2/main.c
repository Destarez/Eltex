#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

uint32_t IpToUint32(const char *ip)
{
    uint32_t val = 0;
    uint8_t octet;
    char *ip_copy = strdup(ip);
    char *token = strtok(ip_copy, ".");

    for (int i = 0; i < 4; ++i)
    {
        if (token != NULL)
        {
            octet = (uint8_t)atoi(token);
            val = (val << 8) | octet;
            token = strtok(NULL, ".");
        }
        else
        {
            free(ip_copy);
            return 0;
        }
    }

    free(ip_copy);
    return val;
}

int IsInSubnet(uint32_t ip, uint32_t subnet, uint32_t mask)
{
    return (ip & mask) == (subnet & mask);
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: <IP_address> <subnet_mask> <number_of_packets>\n");
        return EXIT_FAILURE;
    }

    const char *gateway_ip = argv[1];
    const char *subnet_mask = argv[2];
    int N = atoi(argv[3]);

    uint32_t gateway = IpToUint32(gateway_ip);
    uint32_t mask = IpToUint32(subnet_mask);

    if (gateway == 0 || mask == 0)
    {
        printf("Invalid IP or subnet mask format.\n");
        return EXIT_FAILURE;
    }

    srand(time(NULL));

    int own_subnet_count = 0;
    for (int i = 0; i < N; ++i)
    {
        uint32_t random_ip = (rand() % UINT32_MAX);

        if (IsInSubnet(random_ip, gateway, mask))
        {
            own_subnet_count++;
        }
    }

    printf("Packets in the same subnet: %d/%d\n", own_subnet_count, N);

    return EXIT_SUCCESS;
}
