#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

//131.113.108.200:49152


int main()
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
    //myport = 49153;
    datalen = 6;

    memcpy(sbuf, "Hello", datalen);
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





/*
    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        perror("bind");
        exit(1);
    }

    sktlen = sizeof(myskt);
    if ((count = recvfrom(s, rbuf, sizeof(sbuf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }
*/



/*

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        perror("bind");
        exit(1);
    }

    if ((count = recvfrom(s, sbuf, sizeof(sbuf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
    }

*/

    printf("%s\n", sbuf);

    close(s);

    return 0;
}
