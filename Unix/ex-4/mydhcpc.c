#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "mydhcp.h"

int client_status = INIT;
int client_event = CONNECT_NETWORK;
char server_IP_address[16];

int main(int argc, char *argv[])
{
    extern struct proctable ptab_client[];
    struct proctable *pt;
    //int event;

    int s, count, datalen;
    struct sockaddr_in skt;
    char sbuf[512];
    in_port_t port;
    socklen_t sktlen;



    if (argc == 1) {
        fprintf(stderr, "Please input a server IP address.\n");
        exit(1);
    } else if (argc == 2) {
        memcpy(server_IP_address, argv[1], 16);
    } else {
        fprintf(stderr, "Please input a correct syntax.\n");
        exit(1);
    }



    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    port = 51230;
    datalen = 6;

    //memcpy(sbuf, "Hello", datalen);

    //memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    skt.sin_addr.s_addr = inet_addr(server_IP_address);
    /*
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
        perror("sendto");
        exit(1);
    }
    */








    for (;;) {
        client_event = wait_event();
        pt = ptab_client;
        for (pt = ptab_client; pt->status; pt++) {
            if (pt->status == client_status && pt->event == client_event) {
                (*pt->func)();
                break;
            }
            if (pt->status == 0) {
                fprintf(stderr, "error\n");
                //error;
            }
        }
    }


    //mydhcp_discover();

    //→DHCPDISCOVER

    //←DHCPOFFER
    //mydhcp_request();
    //→DHCPREQUEST
    //←DHCPACK

    //→DHCPREQUEST
    //←DHCPACK
    //mydhcp_release_client();
    //→DHCPRELEASE
    return 0;
}
