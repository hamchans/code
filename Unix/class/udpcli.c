#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

//131.113.108.200:49152


int child_pid = 0;
void timer_handler(int pid) {
    printf("(%ld) time is over, child will be killed\n", time(0));
    kill(child_pid, SIGKILL);
}


int main(int argc, char *argv[])
{
    int s, count, datalen, status;
    pid_t pid, result;
    struct sockaddr_in skt;
    struct sockaddr_in myskt;
    char sbuf[512];
    char buf[512];
    in_port_t port;
    in_port_t myport;
    //struct in_addr ipaddr;
    socklen_t sktlen;

    port = 39152;
    myport = 39153;
    datalen = 6;


    //send

    if ((s = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket");
        exit(1);
    }

    memcpy(sbuf, "Hello", datalen);
    //ipaddr.s_addr = htonl(inet_aton("131.113.108.200", &ipaddr));

    memset(&skt, 0, sizeof(skt));
    skt.sin_family = AF_INET;
    skt.sin_port = htons(port);
    //skt.sin_addr.s_addr = htonl(ipaddr.s_addr);
    skt.sin_addr.s_addr = inet_addr(argv[1]);
    if ((count = sendto(s, sbuf, datalen, 0, (struct sockaddr *)&skt, sizeof(skt))) < 0) {
        perror("sendto");
        exit(1);
    }

    printf("%s\n", sbuf);

    //receive

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

    signal(SIGALRM, timer_handler);

    if ((child_pid = pid = fork()) < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { //child
            if ((count = recvfrom(s, buf, sizeof(buf), 0, (struct sockaddr *)&myskt, &sktlen)) < 0) {
                perror("recvfrom");
                exit(EXIT_FAILURE);
            }
            printf("receiving\n");
            printf("%s\n", buf);
            sleep(10);
        } else {
            alarm(5);
            waitpid(pid, &status, 0);
            exit(0);
        }

    close(s);

    return 0;
}
