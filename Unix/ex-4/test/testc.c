#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mydhcptest.h"

int client_status = INIT;
int client_event = CONNECT_NETWORK;
char server_IP_address[16];

int release_flag = 0;


int main(int argc, char *argv[])
{
    extern struct proctable ptab_client[];
    struct proctable *pt;

    int s, rs;
    struct sockaddr_in skt;
    struct sockaddr_in myskt;
    in_port_t port;
    in_port_t myport;
    socklen_t sktlen;

    struct mydhcp_format format;

    port = 51230;
    myport = 51230;

    if (argc == 1) {
        fprintf(stderr, "Please input a server IP address.\n");
        exit(1);
    } else if (argc == 2) {
        memcpy(server_IP_address, argv[1], 16);
    } else {
        fprintf(stderr, "Please input a correct syntax.\n");
        exit(1);
    }

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

    format.source_IP = 0; //unsigned int
    format.dest_IP = 0; //unsigned int
    format.source_port = 0; //unsigned int
    format.dest_port = 0; //unsigned int
    format.mydhcp_message.type = 0; //unsigned char
    format.mydhcp_message.code = 0; //unsigned char
    format.mydhcp_message.ttl = 0; //unsigned short
    format.mydhcp_message.IP = 0; //unsigned short
    format.mydhcp_message.Netmask = 0; //unsigned int

//ここまで準備
    for (;;) {
        client_event = wait_client_event();
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
