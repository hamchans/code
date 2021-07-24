#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>

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
    printf("Sent to %d:%d\n", format.dest_IP, format.dest_port);
}

struct myftp_format my_recv(int s, struct myftp_format format)
{
    int count;
    if ((count = recv(s, &format, sizeof(format), 0)) < 0) {
        perror("recv");
        exit(1);
    }

    printf("Recv from %d:%d\n", format.source_IP, format.source_port);

    switch (format.myftp_message.type) {
        case TYPE_QUIT:
            printf("Recv quit\n");
            exit(EXIT_SUCCESS);
            break;
        case TYPE_PWD:
            printf("Recv pwd\n");
            my_pwd_server(s);
            break;
        case TYPE_CWD:
            printf("Recv cwd\n");
            my_cd_server(s, format);
            break;
        case TYPE_LIST:
            printf("Recv list\n");
            my_ls_server(s, format);
            break;
        case TYPE_RETR:
            printf("Recv retr\n");
            my_get_server(s, format);
            break;
        case TYPE_STOR:
            printf("Recv stor\n");
            my_put_server(s, format);
            break;
        case TYPE_DATA:
            printf("Recv data\n");
            break;
        default:
            printf("Recv unknown\n");
            exit(EXIT_FAILURE);
            break;
    }
    return format;
}
