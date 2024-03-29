#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "myftp.h"

struct myftp_format format;

char *server_IP_address;
struct in_addr server_net_IP_address;
char *client_IP_address;
struct in_addr client_net_IP_address;
in_port_t port;
in_port_t myport;

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

    int fd;
    struct ifreq ifr;

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
    server_IP_address = ip;
    inet_aton(server_IP_address, &server_net_IP_address);
    //printf("%s\n", ip);
    //printf("%s\n", server_IP_address);

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    client_IP_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    inet_aton(client_IP_address, &client_net_IP_address);

    //ソケット関連
    int s;
    struct sockaddr_in skt;
    //in_port_t port;
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
        perror("connect");
        exit(EXIT_FAILURE);
    }
    format = my_recv(s, format);
    myport = format.dest_port;
    printf("Connection to %s:%d is successful\n", server_IP_address, ntohs(skt.sin_port));

    //my_send(s, format);

    while (1) {
        setargs(&argsc, argsv, lbuf);
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
