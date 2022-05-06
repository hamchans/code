#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int s, count, datalen;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    char buf[512];
    char sbuf[512];
    in_port_t myport;
    in_port_t port;
    socklen_t sktlen;

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    myport = 39152;
    port = 39153;
    datalen = 6;

    memset(&myskt, 0, sizeof(myskt));
    myskt.sin_family = AF_INET;
    myskt.sin_port = htons(myport);
    myskt.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(s, (struct sockaddr *)&myskt, sizeof(myskt)) < 0) {
        perror("bind");
        exit(1);
    }

    sktlen = sizeof(skt);

    while (1) {
        if ((count = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&skt, &sktlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("%s\n", buf);
        memset(sbuf, '\0', 32);
        memcpy(sbuf, "ACK ", 5);
        strcat(sbuf, buf);
        //skt.sin_family = AF_INET;
        skt.sin_port = htons(port);
        //skt.sin_addr.s_addr = inet_addr("210.137.32.218");
        if ((count = sendto(s, sbuf, 24, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
            perror("sendto");
            exit(1);
        }
        sleep(1);

        printf("%s\n", sbuf);
    }



    close(s);

    return 0;
}
