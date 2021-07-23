#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "myftp.h"

void my_send(int s, struct myftp_format format)
{
    int count;
    int datalen;
    datalen = sizeof(format);

    if ((count = send(s, &format, datalen, 0)) < 0) {
        perror("send");
        exit(1);
    }
    printf("Sent %s\n", format.myftp_message.message);
}

void my_recv(int s, struct myftp_format format)
{
    int count;
    if ((count = recv(s, &format, sizeof(format), 0)) < 0) {
        perror("recv");
        exit(1);
    }
    printf("Recv %s\n", format.myftp_message.message);
}
