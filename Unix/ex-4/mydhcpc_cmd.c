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
    {INIT, CONNECT_NETWORK, mydhcp_discover},
    {WAIT_OFFER, RECEIVE_OFFER, mydhcp_request},
    {WAIT_ACK, RECEIVE_ACK, mydhcp_getting_ip},
    {HAVE_IP, HALF_LIMIT, mydhcp_select},
    {HAVE_IP, LIMIT, mydhcp_release_client}
};

void mydhcp_discover()
{
    int s, count, datalen;
    struct sockaddr_in skt;
    struct sockaddr_in myskt;
    char sbuf[512];
    char rbuf[512];
    in_port_t port;
    in_port_t myport;
    //struct in_addr ipaddr;
    socklen_t sktlen;
    extern char server_IP_address;
    //char ipaddr[16];
    //memcpy(ipaddr, "131.113.108.53", 16);



    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    port = 49152;
    myport = 49153;
    datalen = 14;

    memcpy(sbuf, "DHCP DISCOVER", datalen);
    //ipaddr.s_addr = htonl(inet_aton("131.113.108.200", &ipaddr));

    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    //skt.sin_addr.s_addr = htonl(ipaddr.s_addr);
    //skt.sin_addr.s_addr = inet_addr("131.113.108.53");
    skt.sin_addr.s_addr = inet_addr(&server_IP_address);
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    printf("Send %s\n", sbuf);

    close(s);

    mydhcp_wait_offer();

    //extern int client_status;
    //client_status = WAIT_OFFER;
}

void mydhcp_wait_offer()
{
    int s, count, datalen;
    struct sockaddr_in skt;
    struct sockaddr_in myskt;
    char sbuf[512];
    char rbuf[512];
    in_port_t port;
    in_port_t myport;
    //struct in_addr ipaddr;
    socklen_t sktlen;

    extern int client_status;
    client_status = WAIT_OFFER;

    port = 49152;
    myport = 49153;
    datalen = 14;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        //perror("bind");
        //exit(1);
    }

    sktlen = sizeof(myskt);
    if ((count = recvfrom(s, rbuf, sizeof(sbuf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }

    extern int client_event;
    client_event = RECEIVE_OFFER;

    printf("Receive %s\n", rbuf);

    close(s);
}

void mydhcp_request()
{
    int s, count, datalen;
    struct sockaddr_in skt;
    struct sockaddr_in myskt;
    char sbuf[512];
    char rbuf[512];
    in_port_t port;
    in_port_t myport;
    //struct in_addr ipaddr;
    socklen_t sktlen;


    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    port = 49152;
    myport = 49153;
    datalen = 13;

    memcpy(sbuf, "DHCP REQUEST", datalen);
    //ipaddr.s_addr = htonl(inet_aton("131.113.108.200", &ipaddr));

    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    //skt.sin_addr.s_addr = htonl(ipaddr.s_addr);
    skt.sin_addr.s_addr = inet_addr("131.113.108.53");
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    printf("Send %s\n", sbuf);

    close(s);

    mydhcp_wait_ack();

    //extern int client_status;
    //client_status = PREPARE_RELEASE;
}

void mydhcp_wait_ack()
{
    int s, count, datalen;
    struct sockaddr_in skt;
    struct sockaddr_in myskt;
    char sbuf[512];
    char rbuf[512];
    in_port_t port;
    in_port_t myport;
    //struct in_addr ipaddr;
    socklen_t sktlen;

    extern int client_status;
    client_status = WAIT_ACK;

    port = 49152;
    myport = 49153;
    datalen = 13;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        //perror("bind");
        //exit(1);
    }

    sktlen = sizeof(myskt);
    if ((count = recvfrom(s, rbuf, sizeof(sbuf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }

    printf("Receive %s\n", rbuf);

    close(s);

    extern int client_event;
    client_event = RECEIVE_ACK;
}

void mydhcp_getting_ip()
{
    sleep(3);
    extern int client_status;
    client_status = HAVE_IP;

    extern int client_event;
    client_event = HALF_LIMIT;
}

void mydhcp_select()
{
    int select = 0;
    if (select == 0) {
        mydhcp_release_client();
    } else if (select == 1) {
        mydhcp_request();
    }
}

void mydhcp_release_client()
{
    int s, count, datalen;
    struct sockaddr_in skt;
    struct sockaddr_in myskt;
    char sbuf[512];
    char rbuf[512];
    in_port_t port;
    in_port_t myport;
    //struct in_addr ipaddr;
    socklen_t sktlen;


    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    port = 49152;
    myport = 49153;
    datalen = 14;

    memcpy(sbuf, "DHCP RELEASE", datalen);
    //ipaddr.s_addr = htonl(inet_aton("131.113.108.200", &ipaddr));

    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    //skt.sin_addr.s_addr = htonl(ipaddr.s_addr);
    skt.sin_addr.s_addr = inet_addr("131.113.108.53");
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    printf("Send %s\n", sbuf);

    close(s);

    extern int client_status;
    client_status = FIN;
}

int wait_event()
{
    extern int client_event;
    return client_event;
}
