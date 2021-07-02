#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/select.h>
#include <ctype.h>
#include "mydhcptest.h"

struct proctable ptab_server[] = {
    {INIT, START, mydhcp_wait_discover},
    {INIT, RECEIVE_DISCOVER, mydhcp_offer},
    {WAIT_REQUEST, SEND_OFFER, mydhcp_wait_request},
    {WAIT_REQUEST, RECEIVE_REQUEST, mydhcp_ack},
    {ASSIGN_IP, SEND_ACK, mydhcp_in_use_server},
    {ASSIGN_IP, RECEIVE_REQUEST, mydhcp_ack},
    {ASSIGN_IP, RECEIVE_RELEASE, mydhcp_release_server},
    {INIT, RELEASE_IP, mydhcp_wait_discover}
};

void mydhcp_wait_discover(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    int count, datalen;
    char rbuf[512];
    socklen_t sktlen;
    sktlen = sizeof(*myskt);

    if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }
    printf("\n## %s received ##\n", rbuf);

    server_event = RECEIVE_DISCOVER;
}

void mydhcp_offer(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    int count, datalen;
    char sbuf[512];
    datalen = 16;

    memcpy(sbuf, "DHCP OFFER (OK)", datalen);
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&(*myskt), sizeof(*myskt))) < 0) {
        perror("sendto");
        exit(1);
    }
    printf("** %s sent **\n", sbuf);

    server_status = WAIT_REQUEST;
    server_event = SEND_OFFER;
}

void mydhcp_wait_request(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    int count, datalen;
    char rbuf[512];
    socklen_t sktlen;
    sktlen = sizeof(*myskt);

    if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }
    printf("\n## %s received ##\n", rbuf);

    server_event = RECEIVE_REQUEST;
}



void mydhcp_ack(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    int count, datalen;
    char sbuf[512];
    datalen = 14;

    memcpy(sbuf, "DHCP ACK (OK)", datalen);
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&(*myskt), sizeof(*myskt))) < 0) {
        perror("sendto");
        exit(1);
    }
    printf("** %s sent **\n", sbuf);

    server_status = ASSIGN_IP;
    server_event = SEND_ACK;
}

void mydhcp_in_use_server(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    extern int limit_time;
    extern int sigalarm_flag;
    sigalarm_flag = 0;

    int i;

    int count, datalen;
    char rbuf[512];
    char sbuf[512];
    socklen_t sktlen;
    fd_set rdfds;
    struct timeval timeout;
    sktlen = sizeof(*myskt);
    int ret_select;

    datalen = 9;

    while (1) {
        FD_ZERO(&rdfds);
        FD_SET(0, &rdfds);
        FD_SET(s, &rdfds);

        timeout.tv_sec = limit_time / 2;
        timeout.tv_usec = 0;

        ret_select = select(s+1, &rdfds, NULL, NULL, &timeout);

        if (ret_select == -1) {
            printf("Error\n");
            exit(1);
            //error
        }

        if (ret_select == 0) {
            sigalarm_flag++;
            if (sigalarm_flag == 1) {
                memcpy(sbuf, "SIGALARM", datalen);
                if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&(*myskt), sizeof(*myskt))) < 0) {
                    perror("sendto");
                    exit(1);
                }
                printf("** %s sent **\n", sbuf);
                printf("TTL %d\n", limit_time / 2);
                continue;
            } else if (sigalarm_flag == 2) {
                printf("timeout\n");
                server_event = RECEIVE_RELEASE;
                break;
            }
        }

        if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("\n## %s received ##\n", rbuf);

        if (strcmp(rbuf, "DHCP REQUEST") == 0) {
            server_event = RECEIVE_REQUEST;
            break;
        } else if (strcmp(rbuf, "DHCP RELEASE") == 0) {
            server_event = RECEIVE_RELEASE;
            break;
        }
    }


}

void mydhcp_release_server(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    int count, datalen;
    char sbuf[512];
    datalen = 16;

    memcpy(sbuf, "DHCP RELEASE IP", datalen);

    printf("%s\n", sbuf);

    server_status = INIT;
    server_event = RELEASE_IP;

    //exit(0);
}

int wait_server_event()
{
    extern int server_event;
    return server_event;
}

void getargs(int *argsc, char *argsv[], char *lbuf)
{
    int i;
    argsv[(*argsc)++] = lbuf++;
    while (1) {
        if (*lbuf == '\0')
            argsv[(*argsc)++] = ++lbuf;
            if (*lbuf == '\0') {
                argsv[--(*argsc)] = NULL;
                return;
            }
        lbuf++;
    }
}
