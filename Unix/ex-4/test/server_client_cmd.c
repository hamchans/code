#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/time.h>
#include <signal.h>
#include <sys/select.h>
#include <ctype.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include "mydhcptest.h"

struct mydhcp_format set_format(unsigned int source_IP, unsigned int dest_IP, unsigned int source_port, unsigned int dest_port, unsigned char type, unsigned char code, unsigned short ttl, unsigned int IP, unsigned int Netmask)
{
    struct mydhcp_format format;
    format.source_IP = source_IP; //unsigned int
    format.dest_IP = dest_IP; //unsigned int
    format.source_port = source_port; //unsigned int
    format.dest_port = dest_port; //unsigned int
    format.mydhcp_message.type = type; //unsigned char
    format.mydhcp_message.code = code; //unsigned char
    format.mydhcp_message.ttl = ttl; //unsigned short
    format.mydhcp_message.IP = IP; //unsigned short
    format.mydhcp_message.Netmask = Netmask; //unsigned int
    return format;
}
