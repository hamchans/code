#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "mydhcptest.h"

struct proctable ptab_client[] = {
    {INIT, CONNECT_NETWORK, mydhcp_discover},
    {WAIT_OFFER, RECEIVE_OFFER, mydhcp_request},
    {WAIT_ACK, RECEIVE_ACK, mydhcp_in_use_client},
    {HAVE_IP, RECEIVE_ACK, mydhcp_in_use_client},
    {WAIT_OFFER, RECEIVE_OFFER_NG, terminate},
    {WAIT_ACK, RECEIVE_ACK_NG, terminate},
    {HAVE_IP, RECEIVE_SIGALARM, mydhcp_request}
};

void terminate()
{
    exit(0);
}

void proc_release()
{
    extern int release_flag;
    release_flag = 1;
}

void mydhcp_discover(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;

    extern struct mydhcp_format format;
    extern char *server_IP_address;
    extern char *client_IP_address;
    extern in_port_t port;
    extern in_port_t myport;

    char *message;
    char *code_message;

    int count, datalen;
    char sbuf[512];
    socklen_t sktlen;

    struct in_addr client_net_IP_address;
    struct in_addr server_net_IP_address;

    inet_aton(server_IP_address, &server_net_IP_address);
    inet_aton(client_IP_address, &client_net_IP_address);

    format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, myport, port, 1, 0, 0, 0, 0);

    datalen = 160;
    //memcpy(sbuf, "DHCP DISCOVER", 14);
    if ((count = sendto(s, &format, datalen, 0, (struct sockaddr *)&(*skt), sizeof(*skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    switch (format.mydhcp_message.type) {
        case 1:
            client_event = ERROR;
            message = "DISCOVER";
            break;
        case 2:
            client_event = RECEIVE_OFFER;
            message = "OFFER";
            break;
        case 3:
            client_event = ERROR;
            message = "REQUEST";
            break;
        case 4:
            client_event = RECEIVE_ACK;
            message = "ACK";
            break;
        case 5:
            client_event = ERROR;
            message = "RELEASE";
            break;
        default:
            client_event = ERROR;
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
    printf("## message sent to %s:%d ##\n", server_IP_address, format.dest_port);
    printf("type %d(%s), code %d(%s), ttl %d, IP 0.0.0.0, netmask 0.0.0.0\n", format.mydhcp_message.type, message, format.mydhcp_message.code, code_message, format.mydhcp_message.ttl);

    set_client_status(WAIT_OFFER);
    //client_status = WAIT_OFFER;
    //client_event = SEND_DISCOVER;
}

void mydhcp_request(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;

    extern struct mydhcp_format format;
    extern char *server_IP_address;
    extern char *client_IP_address;
    extern in_port_t port;
    extern in_port_t myport;

    char *message;
    char *code_message;

    extern struct in_addr request_IP;
    extern struct in_addr request_Netmask;

    extern int flag;
    int fcode;
    extern int request_ttl;

    int count, datalen;
    char sbuf[512];
    socklen_t sktlen;

    struct in_addr client_net_IP_address;
    struct in_addr server_net_IP_address;

    char str_buffer_ip[INET_ADDRSTRLEN];
    char str_buffer_netmask[INET_ADDRSTRLEN];
    char *ip_addrstr;
    char *netmaskstr;


    inet_aton(server_IP_address, &server_net_IP_address);
    inet_aton(client_IP_address, &client_net_IP_address);

    if (flag == 0) {
        fcode = 2;
    } else if (flag == 1) {
        fcode = 3;
    }

    format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, myport, port, 3, fcode, request_ttl, request_IP.s_addr, request_Netmask.s_addr);

    datalen = 160;
    //memcpy(sbuf, "DHCP REQUEST", 13);
    if ((count = sendto(s, &format, datalen, 0, (struct sockaddr *)&(*skt), sizeof(*skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    switch (format.mydhcp_message.type) {
        case 1:
            client_event = ERROR;
            message = "DISCOVER";
            break;
        case 2:
            client_event = RECEIVE_OFFER;
            message = "OFFER";
            break;
        case 3:
            client_event = ERROR;
            message = "REQUEST";
            break;
        case 4:
            client_event = RECEIVE_ACK;
            message = "ACK";
            break;
        case 5:
            client_event = ERROR;
            message = "RELEASE";
            break;
        default:
            client_event = ERROR;
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
    printf("## message sent to %s:%d ##\n", server_IP_address, format.dest_port);
    printf("type %d(%s), code %d(%s), ttl %d, IP %s, netmask %s\n", format.mydhcp_message.type, message, format.mydhcp_message.code, code_message, format.mydhcp_message.ttl, ip_addrstr, netmaskstr);

    //client_status = WAIT_ACK;
    if (flag == 1) {
        set_client_status(HAVE_IP);
        flag = 0;
    } else {
        set_client_status(WAIT_ACK);
    }
}

void mydhcp_in_use_client(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;
    extern int release_flag;
    struct timeval start_time, end_time;
    extern int flag;

    int count, datalen;
    char rbuf[512];
    socklen_t sktlen;
    sktlen = sizeof(*myskt);

    printf("This IP address is in use.\n");
    set_client_status(HAVE_IP);

    signal(SIGHUP, proc_release);
    while (1) {
        if (release_flag == 1) {
            mydhcp_release_client(s, skt, myskt);
            release_flag = 0;
            exit(0);
        }

        if ((count = recvfrom(s, rbuf, sizeof(rbuf), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
            perror("recvfrom");
            exit(1);
        }
        printf("\n## %s received ##\n", rbuf);

        if (strcmp(rbuf, "SIGALRM") == 0) {
            //client_status = HAVE_IP;
            //set_client_status(HAVE_IP);
            client_event = RECEIVE_SIGALARM;
            flag = 1;
            return;
        }

    }
}

void mydhcp_release_client(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt)
{
    extern int client_status;
    extern int client_event;

    extern struct mydhcp_format format;
    extern char *server_IP_address;
    extern char *client_IP_address;
    extern in_port_t port;
    extern in_port_t myport;

    int count, datalen;
    char sbuf[512];
    socklen_t sktlen;

    struct in_addr client_net_IP_address;
    struct in_addr server_net_IP_address;

    inet_aton(server_IP_address, &server_net_IP_address);
    inet_aton(client_IP_address, &client_net_IP_address);

    format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, myport, port, 5, 0, 0, 0, 0);

    datalen = 160;
    memcpy(sbuf, "DHCP RELEASE", 13);
    if ((count = sendto(s, &format, datalen, 0, (struct sockaddr *)&(*skt), sizeof(*skt))) < 0) {
        perror("sendto");
        exit(1);
    }
    printf("** %s sent **\n", sbuf);

    //client_status = FIN;
    set_client_status(FIN);
    client_event = SEND_RELEASE;
}

void wait_client_event(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt, int st)
{
    extern int client_status;
    extern int client_event;

    extern int flag;
    extern int request_ttl;
    extern struct in_addr request_IP;
    extern struct in_addr request_Netmask;

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
    char rbuf[512];
    socklen_t sktlen;
    sktlen = sizeof(*myskt);

    if (st == 0) {
        return;
    }
    if (flag == 1) {
        return;
    }

    if ((count = recvfrom(s, &format, sizeof(format), 0, (struct sockaddr *)&(*myskt), &sktlen)) < 0) {
        perror("recvfrom");
        exit(1);
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

    switch (format.mydhcp_message.type) {
        case 1:
            client_event = ERROR;
            message = "DISCOVER";
            break;
        case 2:
            client_event = RECEIVE_OFFER;
            message = "OFFER";
            request_ttl = format.mydhcp_message.ttl;
            request_IP.s_addr = format.mydhcp_message.IP;
            request_Netmask.s_addr = format.mydhcp_message.Netmask;
            break;
        case 3:
            client_event = ERROR;
            message = "REQUEST";
            break;
        case 4:
            client_event = RECEIVE_ACK;
            message = "ACK";
            break;
        case 5:
            client_event = ERROR;
            message = "RELEASE";
            break;
        default:
            client_event = ERROR;
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
    if (format.mydhcp_message.code == 4) {
        exit(1);
    }
}

void set_client_status(int changed_status)
{
    extern int client_status;
    switch (client_status) {
        case INIT:
            printf("INIT -> ");
            break;
        case WAIT_OFFER:
            printf("WAIT_OFFER -> ");
            break;
        case WAIT_ACK:
            printf("WAIT_ACK -> ");
            break;
        case HAVE_IP:
            printf("HAVE_IP -> ");
            break;
    }
    client_status = changed_status;
    switch (client_status) {
        case INIT:
            printf("INIT\n");
            break;
        case WAIT_OFFER:
            printf("WAIT_OFFER\n");
            break;
        case WAIT_ACK:
            printf("WAIT_ACK\n");
            break;
        case HAVE_IP:
            printf("HAVE_IP\n");
            break;
    }
}
