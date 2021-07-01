#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mydhcptest.h"

struct proctable ptab_client[] = {
    {INIT, CONNECT_NETWORK, mydhcp_discover},
    {WAIT_OFFER, SEND_DISCOVER, mydhcp_wait_offer},
    {WAIT_OFFER, RECEIVE_OFFER, mydhcp_request},
    {WAIT_ACK, SEND_REQUEST, mydhcp_wait_ack}
    {WAIT_ACK, RECEIVE_ACK, mydhcp_in_use},
    //{HAVE_IP, HALF_LIMIT, mydhcp_select},
    //{HAVE_IP, LIMIT, mydhcp_release_client}
};

void mydhcp_discover(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;

    int count, datalen;
    char sbuf[512];
    socklen_t sktlen;

    datalen = 14;
    memcpy(sbuf, "DHCP DISCOVER", datalen);
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&(*skt), sizeof(*skt))) < 0) {
        perror("sendto");
        exit(1);
    }
    printf("Send %s\n", sbuf);

    client_status = WAIT_OFFER;
    client_event = SEND_DISCOVER;
}

void mydhcp_wait_offer(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;

    int count, datalen;
    char rbuf[512];
    socklen_t sktlen;
    sktlen = sizeof(*myskt);

    if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }
    printf("Receive %s\n", rbuf);

    client_event = RECEIVE_OFFER;
}


void mydhcp_request(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;

    int count, datalen;
    char sbuf[512];
    socklen_t sktlen;

    datalen = 13;
    memcpy(sbuf, "DHCP REQUEST", datalen);
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&(*skt), sizeof(*skt))) < 0) {
        perror("sendto");
        exit(1);
    }
    printf("Send %s\n", sbuf);

    client_status = WAIT_ACK;
    client_event = SEND_REQUEST;
}


void mydhcp_wait_ack(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;

    int count, datalen;
    char rbuf[512];
    socklen_t sktlen;
    sktlen = sizeof(*myskt);

    if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }
    printf("Receive %s\n", rbuf);

    client_status = HAVE_IP;
    client_event = RECEIVE_ACK;

    exit(0);
}

void mydhcp_in_use(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    client_status = HAVE_IP;
    printf("This IP address is in use.\n");
}
/*
void mydhcp_select(int s, struct sockaddr *skt)
{
    int select = 0;
    if (select == 0) {
        mydhcp_release_client();
    } else if (select == 1) {
        mydhcp_request();
    }
}

void mydhcp_release_client(int s, struct sockaddr *skt)
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
*/
int wait_client_event()
{
    extern int client_event;
    return client_event;
}
