#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "myftp.h"

struct myftp_format set_format(unsigned int source_IP, unsigned int dest_IP, unsigned int source_port, unsigned int dest_port, unsigned char type, unsigned char code, unsigned short datalen, char *message)
{
    struct myftp_format format;
    format.source_IP = source_IP; //unsigned int
    format.dest_IP = dest_IP; //unsigned int
    format.source_port = source_port; //unsigned int
    format.dest_port = dest_port; //unsigned int
    format.myftp_message.type = type; //unsigned char
    format.myftp_message.code = code; //unsigned char
    format.myftp_message.data_length = datalen; //unsigned short
    memcpy(format.myftp_message.message, message, 64);
    return format;
}
