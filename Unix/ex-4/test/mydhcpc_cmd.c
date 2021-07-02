#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include "mydhcptest.h"

struct proctable ptab_client[] = {
    {INIT, CONNECT_NETWORK, mydhcp_discover},
    {WAIT_OFFER, SEND_DISCOVER, mydhcp_wait_offer},
    {WAIT_OFFER, RECEIVE_OFFER, mydhcp_request},
    {WAIT_ACK, SEND_REQUEST, mydhcp_wait_ack},
    {WAIT_ACK, RECEIVE_ACK, mydhcp_in_use_client},
    {HAVE_IP, IN_USE, mydhcp_in_use_client},
    {WAIT_OFFER, RECEIVE_OFFER_NG, terminate},
    {WAIT_ACK, RECEIVE_ACK_NG, terminate},
    {HAVE_IP, RECEIVE_SIGALARM, mydhcp_request}
};

void terminate()
{
    exit(0);
}

void proc_release()
{
    extern int release_flag;
    release_flag = 1;
}

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
    printf("** %s sent **\n", sbuf);

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
    printf("\n## %s received ##\n", rbuf);

    if (strcmp(rbuf, "DHCP OFFER (OK)") == 0) {
        client_event = RECEIVE_OFFER;
    } else if (strcmp(rbuf, "DHCP OFFER NG") == 0) {
        client_event = RECEIVE_OFFER_NG;
    }

    //client_event = RECEIVE_OFFER;
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
    printf("** %s sent **\n", sbuf);

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
    printf("\n## %s received ##\n", rbuf);

    if (strcmp(rbuf, "DHCP ACK (OK)") == 0) {
        client_event = RECEIVE_ACK;
    } else if (strcmp(rbuf, "DHCP ACK NG") == 0) {
        client_event = RECEIVE_ACK_NG;
    }

    //client_event = RECEIVE_ACK;
}

void mydhcp_in_use_client(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;
    extern int release_flag;
    struct timeval start_time, end_time;

    int count, datalen;
    char rbuf[512];
    socklen_t sktlen;
    sktlen = sizeof(*myskt);

    printf("This IP address is in use.\n");

    signal(SIGHUP, proc_release);
    //gettimeofday(&start_time, NULL);
    while (1) {
        if (release_flag == 1) {
            mydhcp_release_client(s, skt, myskt);
            release_flag = 0;
            exit(0);
        }

        if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("\n## %s received ##\n", rbuf);

        if (strcmp(rbuf, "SIGALARM") == 0) {
            client_status = HAVE_IP;
            client_event = RECEIVE_SIGALARM;
            return;
        }

    }
    client_status = HAVE_IP;
    client_event = IN_USE;
}

void mydhcp_release_client(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;

    int count, datalen;
    char sbuf[512];
    socklen_t sktlen;

    datalen = 13;
    memcpy(sbuf, "DHCP RELEASE", datalen);
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&(*skt), sizeof(*skt))) < 0) {
        perror("sendto");
        exit(1);
    }
    printf("** %s sent **\n", sbuf);

    client_status = FIN;
    client_event = SEND_RELEASE;
}

int wait_client_event()
{
    extern int client_event;
    return client_event;
}
