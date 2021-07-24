#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "myftp.h"

struct myftp_format format;

char *server_IP_address;
char *client_IP_address;

int main(int argc, char *argv[])
{
    int s, s2;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    in_port_t myport;
    socklen_t sktlen;
    int count;
    char rbuf[256];

    my_cd(argc, argv, 0);

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    myport = 62098;

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

    while (1) {
        my_recv(s2, format);
    }

    close(s);
    close(s2);

    return 0;
}
