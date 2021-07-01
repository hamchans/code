#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mydhcp.h"

struct proctable ptab_client[] = {
    {INIT, START, mydhcp_wait_discover},
    {WAIT_DISCOVER, RECEIVE_DISCOVER, mydhcp_offer},
    {WAIT_REQUEST, RECEIVE_REQUEST, mydhcp_ack},
    {ASSIGN_IP, RECEIVE_REQUEST, mydhcp_ack},
    {ASSIGN_IP, RECEIVE_RELEASE, mydhcp_release_server}
};


void mydhcp_wait_discover()
{
    int s, count, datalen;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    char rbuf[512];
    char sbuf[512];
    in_port_t myport;
    in_port_t port;
    socklen_t sktlen;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    myport = 49152;
    port = 49153;
    datalen = 11;

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        //perror("bind");
        //exit(1);
    }

    sktlen = sizeof(skt);
    if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }

    printf("Receive %s\n", rbuf);

    memcpy(sbuf, "DHCP OFFER", datalen);






    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }


    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    skt.sin_addr.s_addr = inet_addr("131.113.108.54");
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    printf("Send %s\n", sbuf);

    close(s);

    extern int server_status;
    server_status = WAIT_REQUEST;

}

void mydhcp_offer()
{

}

void mydhcp_ack()
{
    int s, count, datalen;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    char rbuf[512];
    char sbuf[512];
    in_port_t myport;
    in_port_t port;
    socklen_t sktlen;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    myport = 49152;
    port = 49153;
    datalen = 9;

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        //perror("bind");
        //exit(1);
    }

    sktlen = sizeof(myskt);
    if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }

    printf("Receive %s\n", rbuf);

    memcpy(sbuf, "DHCP ACK", datalen);






    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }


    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    skt.sin_addr.s_addr = inet_addr("131.113.108.54");
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    printf("Send %s\n", sbuf);

    close(s);

    extern int server_status;
    server_status = WAIT_REQUEST;
}

void mydhcp_release_server()
{
    int s, count, datalen;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    char rbuf[512];
    char sbuf[512];
    in_port_t myport;
    in_port_t port;
    socklen_t sktlen;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    myport = 49152;
    port = 49153;
    datalen = 19;

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        //perror("bind");
        //exit(1);
    }

    sktlen = sizeof(skt);
    if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }

    printf("Receive %s\n", rbuf);

    memcpy(sbuf, "RELEASE IP ADDRESS", datalen);






    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }


    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    skt.sin_addr.s_addr = inet_addr("131.113.108.54");
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    printf("Send %s\n", sbuf);

    close(s);

    extern int server_status;
    server_status = WAIT_REQUEST;
}
