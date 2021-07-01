#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mydhcp.h"

void prepare_sender()
{
    extern int ss;
    extern struct sockaddr_in skt;
    extern char sbuf[512];
    extern in_port_t port;
    extern socklen_t sktlen;
    extern char server_IP_address;

    if ((ss = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    port = 51230;
    //datalen = 14;

    //memcpy(sbuf, "TEST SENDER", datalen);

    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    skt.sin_addr.s_addr = inet_addr(&server_IP_address);
}

void prepare_receiver()
{
    extern int rs;
    extern struct sockaddr_in myskt;
    extern char rbuf[512];
    extern in_port_t myport;

    if ((rs = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    myport = 51230;

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(rs, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        perror("bind");
        exit(1);
    }
}
