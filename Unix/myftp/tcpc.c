#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "myftp.h"

int main(int argc, char *argv[]) {
    int s;
    struct sockaddr_in skt;
    in_port_t port;
    struct in_addr ipaddr;
    int count, datalen;
    char sbuf[256];
    char rbuf[256];

    struct myftp_format format;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    port = 50000;
    //inet_aton(argv[1], &ipaddr);

    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    //skt.sin_addr.s_addr = htonl(ipaddr.s_addr);
    skt.sin_addr.s_addr = inet_addr(argv[1]);

    if (connect(s, (struct sockaddr *)&skt, sizeof(skt)) < 0) {
        fprintf(stderr, "ERROR2\n");
        exit(EXIT_FAILURE);
    }

/*
    if ((count = sendto(s, &format, datalen, 0, (struct sockaddr *)&(*skt), sizeof(*skt))) < 0) {
        perror("sendto");
        exit(1);
    }
*/
    datalen = 6;
    memcpy(sbuf, "Hello", datalen);
    //format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, myport, port, 1, 0, 0, 0, 0);


    if ((count = send(s, &sbuf, datalen, 0)) < 0) {
        perror("send");
        exit(1);
    }
    if ((count = recv(s, &rbuf, sizeof(rbuf), 0)) < 0) {
        perror("recv");
        exit(1);
    }
    printf("%s\n", rbuf);

    close(s);

    return 0;
}
