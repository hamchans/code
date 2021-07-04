#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "mydhcptest.h"

int client_status = INIT;
int client_event = CONNECT_NETWORK;

int release_flag = 0;

struct mydhcp_format format;
char *server_IP_address;
char *client_IP_address;

int flag = 0;
int request_ttl = 4;
struct in_addr request_IP;
struct in_addr request_Netmask;

in_port_t port;
in_port_t myport;


int main(int argc, char *argv[])
{
    extern struct proctable ptab_client[];
    struct proctable *pt;

    int s, rs;
    struct sockaddr_in skt;
    struct sockaddr_in myskt;
    socklen_t sktlen;

    char input_address[16];

    int fd;
    struct ifreq ifr;
    int st = 0;

    port = 51231;
    myport = 51231;

    if (argc == 1) {
        fprintf(stderr, "Please input a server IP address.\n");
        exit(1);
    } else if (argc == 2) {
        memcpy(input_address, argv[1], 16);
    } else {
        fprintf(stderr, "Please input a correct syntax.\n");
        exit(1);
    }


    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    client_IP_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    server_IP_address = input_address;

    printf("-- server IP: %s:%d --\n", server_IP_address, port);

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    skt.sin_addr.s_addr = inet_addr(server_IP_address);


    if ((rs = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(rs, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        perror("bind");
        exit(1);
    }

//ここまで準備
    for (;;) {
        wait_client_event(s, &skt, &myskt, st++);
        pt = ptab_client;
        for (pt = ptab_client; pt->status; pt++) {
            if (pt->status == client_status && pt->event == client_event) {
                (*pt->func)(s, &skt, &myskt);
                break;
            }
            if (pt->status == 0) {
                fprintf(stderr, "error\n");
                //error;
            }
        }
    }

    close(s);
    return 0;
}
