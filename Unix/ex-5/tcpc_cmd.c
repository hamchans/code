#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "myftp.h"

extern char *server_IP_address;
extern struct in_addr server_net_IP_address;
extern char *client_IP_address;
extern struct in_addr client_net_IP_address;


void my_send(int s, struct myftp_format format)
{
    int count;
    int datalen;
    datalen = sizeof(format);

    if ((count = send(s, &format, datalen, 0)) < 0) {
        perror("send");
        exit(1);
    }
    //printf("Sent to %d:%d\n", format.dest_IP, format.dest_port);
}

struct myftp_format my_recv(int s, struct myftp_format format)
{
    int count;
    if ((count = recv(s, &format, sizeof(format), 0)) < 0) {
        perror("recv");
        exit(1);
    }
    //printf("Recv from %d:%d\n", format.source_IP, format.source_port);
    return format;
}
