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
in_port_t myport;
in_port_t port;

int main(int argc, char *argv[])
{
    int s, s2;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    //in_port_t myport;
    socklen_t sktlen;
    int count;
    char rbuf[256];
    char message[DATASIZE];

    int fd;
    struct ifreq ifr;

    my_cd(argc, argv, 0);

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    myport = 62098;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);
    close(fd);
    server_IP_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    inet_aton(server_IP_address, &server_net_IP_address);

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(s, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    sktlen = sizeof(skt);
    if ((s2 = accept(s, (struct sockaddr *)&skt, &sktlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    client_net_IP_address = skt.sin_addr;
    client_IP_address = inet_ntoa(skt.sin_addr);
    port = skt.sin_port;

    format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, 0x00, 0x00, 0, message);
    my_send(s2, format);

    printf("Connection to %s:%d is successful\n", client_IP_address, ntohs(skt.sin_port));

    //printf("sin_port: %d\n", ntohs(skt.sin_port));

    while (1) {
        my_recv(s2, format);
    }

    close(s);
    close(s2);

    return 0;
}
