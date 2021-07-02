#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "mydhcptest.h"

int server_status = INIT;
int server_event = START;

int sigalarm_flag = 0;
int limit_time;

int main(int argc, char *argv[])
{
    extern struct proctable ptab_server[];
    struct proctable *pt;

    int s, rs;
    struct sockaddr_in myskt;
    struct sockaddr_in skt;
    in_port_t myport;
    socklen_t sktlen;

    FILE *fp;
    char filename[12];
    char ch;
    char lbuf[256];
    int p = 0, i = 0;
    char time[16];

    int argsc = 0;
    char *argsv[256];

    myport = 51230;

    if (argc == 1) {
        fprintf(stderr, "Please input a config-file.\n");
        exit(1);
    } else if (argc == 2) {
        memcpy(filename, argv[1], 12);
        if((fp = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "%s file cannot open!\n", filename);
            exit(1);
    	}
    } else {
        fprintf(stderr, "Please input a correct syntax.\n");
        exit(1);
    }

    while ((ch = fgetc(fp)) != '\n')
        time[i++] = ch;
    limit_time = atoi(time);

    while((ch = fgetc(fp)) != EOF) {
        if (ch == ' ') {
            lbuf[p++] = '\0';
            //while ((ch = fgetc(fp)) != '\n');
        } else if (ch == '\n') {
            lbuf[p++] = '\0';
        } else {
            lbuf[p++] = ch;
        }
	}
    lbuf[p++] = '\0';

    getargs(&argsc, argsv, lbuf);

    printf("-- config file: TTL %d --\n", limit_time);
    for (i=0; i<argsc; i+=2) {
        printf("-- config file: IP %s, netmask %s --\n", argsv[i], argsv[i+1]);
    }


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

//ここまで準備
    for (;;) {
        printf("\n-- wait for event --\n");
        server_event = wait_server_event();
        pt = ptab_server;
        for (pt = ptab_server; pt->status; pt++) {
            if (pt->status == server_status && pt->event == server_event) {
                (*pt->func)(s, &skt, &myskt);
                break;
            }
            if (pt->status == 0) {
                fprintf(stderr, "error\n");
                //error;
            }
        }
    }

    close(s);
    return 0;
}
