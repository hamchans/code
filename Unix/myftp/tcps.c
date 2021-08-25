#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "myftp.h"


int main()
{
    int s, s2;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    in_port_t myport;
    socklen_t sktlen;
    int count;
    char rbuf[256];

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    myport = 50000;

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(s, 5) < 0) {
        fprintf(stderr, "ERROR3\n");
        exit(EXIT_FAILURE);
    }

    sktlen = sizeof(skt);
    if ((s2 = accept(s, (struct sockaddr *)&skt, &sktlen)) < 0) {
        fprintf(stderr, "ERROR4\n");
        exit(EXIT_FAILURE);
    }

/*
    if ((count = recvfrom(s, &format, sizeof(format), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }
*/
    if ((count = recv(s2, &rbuf, sizeof(rbuf), 0)) < 0) {
        perror("recv");
        exit(1);
    }
    if ((count = send(s2, &rbuf, 6, 0)) < 0) {
        perror("send");
        exit(1);
    }

    printf("%s\n", rbuf);

    close(s);
    close(s2);

    return 0;
}
