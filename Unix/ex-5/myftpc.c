#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "myftp.h"

struct myftp_format format;

int main(int argc, char *argv[])
{
    int argsc;
    char *argsv[BUFLENGTH], lbuf[NARGS];
    extern struct command_table cmd_tbl[];
    struct command_table *p;

    struct addrinfo hints, *res;
    struct addrinfo *tmp;
    char *host, *serv;
    int err;
    char ip[256];

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;

    host = argv[1];
    serv = "http";

    if (argc == 1) {
        fprintf(stderr, "Please input a host name.\n");
        exit(EXIT_FAILURE);
    } else if (argc > 2) {
        fprintf(stderr, "Please type in the correct syntax.\n");
        exit(EXIT_FAILURE);
    }

    if ((err = getaddrinfo(host, serv, &hints, &res)) < 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        exit(EXIT_FAILURE);
    }

    getnameinfo(res->ai_addr, res->ai_addrlen, ip, sizeof(ip), NULL, 0, NI_NUMERICHOST);
    freeaddrinfo(res);
    printf("%s\n", ip);

    //ソケット関連
    int s;
    struct sockaddr_in skt;
    in_port_t port;
    struct in_addr ipaddr;
    int count, datalen;
    char sbuf[256];
    char rbuf[256];

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    port = 62098;
    //inet_aton(argv[1], &ipaddr);

    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    //skt.sin_addr.s_addr = htonl(ipaddr.s_addr);
    skt.sin_addr.s_addr = inet_addr(ip);

    if (connect(s, (struct sockaddr *)&skt, sizeof(skt)) < 0) {
        fprintf(stderr, "ERROR2\n");
        exit(EXIT_FAILURE);
    }

    //my_send(s, format);

    while (1) {
        printf("myFTP%% ");

        if (fgets(lbuf, sizeof(lbuf), stdin) == NULL) {
            putchar('\n');
            return 0;
        }
        lbuf[strlen(lbuf) - 1] = '\0';
        if (*lbuf == '\0')
            continue;

        getargs(&argsc, argsv, lbuf);

        for (p = cmd_tbl; p->cmd; p++) {
            if (strcmp(argsv[0], p->cmd) == 0) {
                (*p->func)(argsc, argsv, s);
                break;
            }
        }
        if (p->cmd == NULL)
            fprintf(stderr, "Unknown Command\n");
    }

    close(s);

    return 0;
}
