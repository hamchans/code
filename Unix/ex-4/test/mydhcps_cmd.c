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
#include <sys/ioctl.h>
#include <net/if.h>
#include "mydhcptest.h"

struct proctable ptab_server[] = {
    {INIT, RECEIVE_DISCOVER, mydhcp_offer},
    {WAIT_REQUEST, RECEIVE_REQUEST, mydhcp_ack},
    //{ASSIGN_IP, SEND_ACK, mydhcp_in_use_server},
    {ASSIGN_IP, RECEIVE_REQUEST, mydhcp_ack},
    {ASSIGN_IP, RECEIVE_RELEASE, mydhcp_release_server}
};

void mydhcp_offer(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    extern int argsc;
    extern char *argsv[256];
    extern int assign[256];
    extern int limit_time;
    int assign_index;
    extern int fcode;

    struct mydhcp_format format;
    char *server_IP_address;
    char *client_IP_address;
    extern in_port_t myport;
    extern in_port_t port;

    char *message;
    char *code_message;

    int fd;
    struct ifreq ifr;
    struct in_addr client_net_IP_address;
    struct in_addr server_net_IP_address;
    struct in_addr assign_net_IP_address;
    struct in_addr assign_net_Netmask;

    extern struct in_addr offer_IP;
    extern struct in_addr offer_Netmask;

    char str_buffer_ip[INET_ADDRSTRLEN];
    char str_buffer_netmask[INET_ADDRSTRLEN];
    char *ip_addrstr;
    char *netmaskstr;

    int count, datalen;
    char sbuf[512];
    datalen = 160;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    server_IP_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    client_IP_address = "131.113.108.54";
    inet_aton(server_IP_address, &server_net_IP_address);
    inet_aton(client_IP_address, &client_net_IP_address);

    if ((assign_index = is_assign(assign, argsc)) != -1) {
        fcode = 0;
        inet_aton(argsv[assign_index*2], &assign_net_IP_address);
        inet_aton(argsv[assign_index*2+1], &assign_net_Netmask);
    } else {
        fcode = 1;
    }

    offer_IP = assign_net_IP_address;
    offer_Netmask = assign_net_Netmask;

    format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, 2, fcode, limit_time, assign_net_IP_address.s_addr, assign_net_Netmask.s_addr);

    memcpy(sbuf, "DHCP OFFER (OK)", 16);
    if ((count = sendto(s, &format, datalen, 0, (struct sockaddr *)&(*myskt), sizeof(*myskt))) < 0) {
        perror("sendto");
        exit(1);
    }

    switch (format.mydhcp_message.type) {
        case 1:
            server_event = RECEIVE_DISCOVER;
            message = "DISCOVER";
            break;
        case 2:
            server_event = ERROR;
            message = "OFFER";
            break;
        case 3:
            server_event = RECEIVE_REQUEST;
            message = "REQUEST";
            break;
        case 4:
            server_event = ERROR;
            message = "ACK";
            break;
        case 5:
            server_event = RECEIVE_RELEASE;
            message = "RELEASE";
            break;
        default:
            server_event = ERROR;
            message = "UNKNOWN MESSAGE";
            break;
    }

    switch (format.mydhcp_message.code) {
        case 0:
            code_message = "SUCCESS";
            break;
        case 1:
            code_message = "ERROR (NO ALLOC IP)";
            break;
        case 2:
            code_message = "ALLOCREQ";
            break;
        case 3:
            code_message = "EXTREQ";
            break;
        case 4:
            code_message = "ERROR (REQWRONG)";
            break;
        default:
            code_message = "ERROR";
            break;
    }

    struct in_addr ip_addr = {format.mydhcp_message.IP};
	struct in_addr netmask_addr = {format.mydhcp_message.Netmask};
    if (inet_ntop(AF_INET, &ip_addr.s_addr, str_buffer_ip, sizeof(str_buffer_ip)) != NULL) {
        ip_addrstr = str_buffer_ip;
    }
    if (inet_ntop(AF_INET, &netmask_addr.s_addr, str_buffer_netmask, sizeof(str_buffer_netmask)) != NULL) {
        netmaskstr = str_buffer_netmask;
    }

    printf("## message sent to %s:%d ##\n", client_IP_address, format.dest_port);
    printf("type %d(%s), code %d(%s), ttl %d, IP %s, netmask %s\n", format.mydhcp_message.type, message, format.mydhcp_message.code, code_message, format.mydhcp_message.ttl, ip_addrstr, netmaskstr);

    set_server_status(WAIT_REQUEST);
    server_event = SEND_OFFER;
}

void mydhcp_ack(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    extern int fcode;

    struct mydhcp_format format;
    char *server_IP_address;
    char *client_IP_address;
    extern in_port_t myport;
    extern in_port_t port;

    char *message;
    char *code_message;

    int fd;
    struct ifreq ifr;
    struct in_addr client_net_IP_address;
    struct in_addr server_net_IP_address;
    extern struct in_addr request_IP;
    extern struct in_addr request_Netmask;

    char str_buffer_ip[INET_ADDRSTRLEN];
    char str_buffer_netmask[INET_ADDRSTRLEN];
    char *ip_addrstr;
    char *netmaskstr;

    int count, datalen;
    char sbuf[512];
    datalen = 160;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
    ioctl(fd, SIOCGIFADDR, &ifr);

    close(fd);

    server_IP_address = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);
    client_IP_address = "131.113.108.54";
    inet_aton(server_IP_address, &server_net_IP_address);
    inet_aton(client_IP_address, &client_net_IP_address);

    format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, 4, fcode, 0, request_IP.s_addr, request_Netmask.s_addr);

    //memcpy(sbuf, "DHCP ACK (OK)", 14);
    if ((count = sendto(s, &format, datalen, 0, (struct sockaddr *)&(*myskt), sizeof(*myskt))) < 0) {
        perror("sendto");
        exit(1);
    }

    if (fcode == 0) {
        memcpy(sbuf, "DHCP ACK (OK)", 14);
        set_server_status(ASSIGN_IP);
        //server_event = SEND_ACK;
    } else if (fcode == 4) {
        memcpy(sbuf, "DHCP ACK (NG)", 14);
        set_server_status(INIT);
    }

    switch (format.mydhcp_message.type) {
        case 1:
            server_event = RECEIVE_DISCOVER;
            message = "DISCOVER";
            break;
        case 2:
            server_event = ERROR;
            message = "OFFER";
            break;
        case 3:
            server_event = RECEIVE_REQUEST;
            message = "REQUEST";
            break;
        case 4:
            server_event = ERROR;
            message = "ACK";
            break;
        case 5:
            server_event = RECEIVE_RELEASE;
            message = "RELEASE";
            break;
        default:
            server_event = ERROR;
            message = "UNKNOWN MESSAGE";
            break;
    }

    switch (format.mydhcp_message.code) {
        case 0:
            code_message = "SUCCESS";
            break;
        case 1:
            code_message = "ERROR (NO ALLOC IP)";
            break;
        case 2:
            code_message = "ALLOCREQ";
            break;
        case 3:
            code_message = "EXTREQ";
            break;
        case 4:
            code_message = "ERROR (REQWRONG)";
            break;
        default:
            code_message = "ERROR";
            break;
    }

    struct in_addr ip_addr = {format.mydhcp_message.IP};
	struct in_addr netmask_addr = {format.mydhcp_message.Netmask};
    if (inet_ntop(AF_INET, &ip_addr.s_addr, str_buffer_ip, sizeof(str_buffer_ip)) != NULL) {
        ip_addrstr = str_buffer_ip;
    }
    if (inet_ntop(AF_INET, &netmask_addr.s_addr, str_buffer_netmask, sizeof(str_buffer_netmask)) != NULL) {
        netmaskstr = str_buffer_netmask;
    }

    printf("## message sent to %s:%d ##\n", client_IP_address, format.dest_port);
    printf("type %d(%s), code %d(%s), ttl %d, IP %s, netmask %s\n", format.mydhcp_message.type, message, format.mydhcp_message.code, code_message, format.mydhcp_message.ttl, ip_addrstr, netmaskstr);
}
/*
void mydhcp_in_use_server(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    extern int limit_time;
    extern int sigalarm_flag;
    sigalarm_flag = 0;

    int i;

    int count, datalen;
    char rbuf[512];
    char sbuf[512];
    socklen_t sktlen;
    fd_set rdfds;
    struct timeval timeout;
    sktlen = sizeof(*myskt);
    int ret_select;

    datalen = 9;

    while (1) {
        FD_ZERO(&rdfds);
        FD_SET(0, &rdfds);
        FD_SET(s, &rdfds);

        timeout.tv_sec = limit_time / 2;
        timeout.tv_usec = 0;

        ret_select = select(s+1, &rdfds, NULL, NULL, &timeout);

        if (ret_select == -1) {
            printf("Error\n");
            exit(1);
            //error
        }

        if (ret_select == 0) {
            sigalarm_flag++;
            if (sigalarm_flag == 1) {
                memcpy(sbuf, "SIGALRM", datalen);
                if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&(*myskt), sizeof(*myskt))) < 0) {
                    perror("sendto");
                    exit(1);
                }
                printf("** %s sent **\n", sbuf);
                printf("TTL %d\n", limit_time / 2);
                continue;
            } else if (sigalarm_flag == 2) {
                printf("timeout\n");
                server_event = RECEIVE_RELEASE;
                break;
            }
        }

        if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("\n## %s received ##\n", rbuf);

        if (strcmp(rbuf, "DHCP REQUEST") == 0) {
            server_event = RECEIVE_REQUEST;
            break;
        } else if (strcmp(rbuf, "DHCP RELEASE") == 0) {
            server_event = RECEIVE_RELEASE;
            break;
        }
    }
}
*/
void mydhcp_release_server(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    extern struct in_addr offer_IP;
    extern struct in_addr offer_Netmask;

    int count, datalen;
    char sbuf[512];
    datalen = 16;

    memcpy(sbuf, "DHCP RELEASE IP", datalen);

    printf("%s\n", sbuf);

    server_status = INIT;
    server_event = RELEASE_IP;

    offer_IP.s_addr = 0;
    offer_Netmask.s_addr = 0;
}

void wait_server_event(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int server_status;
    extern int server_event;

    extern char *argsv[256];

    extern struct in_addr offer_IP;
    extern struct in_addr offer_Netmask;
    extern struct in_addr request_IP;
    extern struct in_addr request_Netmask;
    extern in_port_t port;

    extern int fcode;

    struct mydhcp_format format;
    char *source_addrstr;
    char *dest_addrstr;
    char *ip_addrstr;
    char *netmaskstr;
    char str_buffer_source[INET_ADDRSTRLEN];
    char str_buffer_dest[INET_ADDRSTRLEN];
    char str_buffer_ip[INET_ADDRSTRLEN];
    char str_buffer_netmask[INET_ADDRSTRLEN];
    char *message;
    char *code_message;

    int count, datalen;
    char sbuf[512];
    char rbuf[512];
    socklen_t sktlen;
    sktlen = sizeof(*myskt);

    extern int limit_time;
    extern int sigalarm_flag;
    sigalarm_flag = 0;

    int i, flag = 0;

    fd_set rdfds;
    struct timeval timeout;
    int ret_select;

    request_IP.s_addr = 0;
    request_Netmask.s_addr = 0;

    while (server_status == ASSIGN_IP) {
        FD_ZERO(&rdfds);
        FD_SET(0, &rdfds);
        FD_SET(s, &rdfds);

        timeout.tv_sec = limit_time / 2;
        timeout.tv_usec = 0;

        ret_select = select(s+1, &rdfds, NULL, NULL, &timeout);

        if (ret_select == -1) {
            printf("Error\n");
            exit(1);
            //error
        }

        if (ret_select == 0) {
            sigalarm_flag++;
            if (sigalarm_flag == 1) {
                memcpy(sbuf, "SIGALRM", 8);
                if ((count = sendto(s, sbuf, 8, 0, (struct sockaddr *)&(*myskt), sizeof(*myskt))) < 0) {
                    perror("sendto");
                    exit(1);
                }
                printf("-- client IP %s, TTL %d --\n", argsv[0], limit_time / 2);
                printf("** %s sent **\n", sbuf);
                continue;
            } else if (sigalarm_flag == 2) {
                printf("timeout\n");
                server_event = RECEIVE_RELEASE;
                return;
            }
        }

        if ((count = recvfrom(s, &format, sizeof(format), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
            perror("recvfrom");
            exit(1);
        } else {
            flag = 1;
            break;
        }
    }

    if (flag == 0) {
        if ((count = recvfrom(s, &format, sizeof(format), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
    }

    struct in_addr source_addr = {format.source_IP};
	struct in_addr dest_addr = {format.dest_IP};
    struct in_addr ip_addr = {format.mydhcp_message.IP};
	struct in_addr netmask_addr = {format.mydhcp_message.Netmask};

    if (inet_ntop(AF_INET, &source_addr.s_addr, str_buffer_source, sizeof(str_buffer_source)) != NULL) {
        source_addrstr = str_buffer_source;
    }
    if (inet_ntop(AF_INET, &dest_addr.s_addr, str_buffer_dest, sizeof(str_buffer_dest)) != NULL) {
        dest_addrstr = str_buffer_dest;
    }
    if (inet_ntop(AF_INET, &ip_addr.s_addr, str_buffer_ip, sizeof(str_buffer_ip)) != NULL) {
        ip_addrstr = str_buffer_ip;
    }
    if (inet_ntop(AF_INET, &netmask_addr.s_addr, str_buffer_netmask, sizeof(str_buffer_netmask)) != NULL) {
        netmaskstr = str_buffer_netmask;
    }

	if (source_addrstr) {
		printf("## message received from %s:%d ##\n", source_addrstr, format.source_port);
	} else {
		//error
	}

    port = format.source_port;

    switch (format.mydhcp_message.type) {
        case 1:
            server_event = RECEIVE_DISCOVER;
            message = "DISCOVER";
            break;
        case 2:
            server_event = ERROR;
            message = "OFFER";
            break;
        case 3:
            server_event = RECEIVE_REQUEST;
            message = "REQUEST";
            if (format.mydhcp_message.code == 2 || format.mydhcp_message.code == 3) {
                if (format.mydhcp_message.IP == offer_IP.s_addr && format.mydhcp_message.Netmask == offer_Netmask.s_addr) {
                    if (format.mydhcp_message.ttl <= limit_time) {
                        fcode = 0;
                        request_IP.s_addr = format.mydhcp_message.IP;
                        request_Netmask.s_addr = format.mydhcp_message.Netmask;
                        break;
                    }
                }
            }
            fcode =  4;
            break;
        case 4:
            server_event = ERROR;
            message = "ACK";
            break;
        case 5:
            server_event = RECEIVE_RELEASE;
            message = "RELEASE";
            break;
        default:
            server_event = ERROR;
            message = "UNKNOWN MESSAGE";
            break;
    }

    switch (format.mydhcp_message.code) {
        case 0:
            code_message = "SUCCESS";
            break;
        case 1:
            code_message = "ERROR (NO ALLOC IP)";
            break;
        case 2:
            code_message = "ALLOCREQ";
            break;
        case 3:
            code_message = "EXTREQ";
            break;
        case 4:
            code_message = "ERROR (REQWRONG)";
            break;
        default:
            code_message = "ERROR";
            break;
    }

    printf("type %d(%s), code %d(%s), ttl %d, IP %s, netmask %s\n", format.mydhcp_message.type, message, format.mydhcp_message.code, code_message, format.mydhcp_message.ttl, ip_addrstr, netmaskstr);
}

void getargs(int *argsc, char *argsv[], char *lbuf)
{
    int i;
    argsv[(*argsc)++] = lbuf++;
    while (1) {
        if (*lbuf == '\0')
            argsv[(*argsc)++] = ++lbuf;
            if (*lbuf == '\0') {
                argsv[--(*argsc)] = NULL;
                return;
            }
        lbuf++;
    }
}

int is_assign(int assign[], int argsc)
{
    int i;

    for (i=0; i<argsc; i++) {
        if (assign[i] == 1) {
            return i;
        }
    }
    return -1;
}

void set_server_status(int changed_status)
{
    extern int server_status;
    switch (server_status) {
        case INIT:
            printf("INIT -> ");
            break;
        case WAIT_REQUEST:
            printf("WAIT_REQUEST -> ");
            break;
        case ASSIGN_IP:
            printf("ASSIGN_IP-> ");
            break;
    }
    server_status = changed_status;
    switch (server_status) {
        case INIT:
            printf("INIT\n");
            break;
        case WAIT_REQUEST:
            printf("WAIT_REQUEST\n");
            break;
        case ASSIGN_IP:
            printf("ASSIGN_IP\n");
            break;
    }
}
