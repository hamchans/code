#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>

#include "myftp.h"

extern char pathname[PATHNAME_SIZE];
extern struct myftp_format format;

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

    switch (format.myftp_message.type) {
        case TYPE_QUIT:
            printf("Recv quit\n");
            exit(EXIT_SUCCESS);
            break;
        case TYPE_PWD:
            printf("Recv pwd\n");
            my_pwd_server();
            break;
        case TYPE_CWD:
            printf("Recv cwd\n");
            my_cd_server(format);
            break;
        case TYPE_LIST:
            printf("Recv list\n");
            my_ls_server(format);
            break;
        case TYPE_RETR:
            printf("Recv retr\n");
            break;
        case TYPE_STOR:
            printf("Recv stor\n");
            break;
        default:
            printf("Recv unknown\n");
            break;
    }
    printf("Recv %s\n", format.myftp_message.message);
}

void my_pwd_server()
{
    getcwd(pathname, PATHNAME_SIZE);
    printf("%s\n", pathname);
    return;
}

void my_cd_server(struct myftp_format format)
{
    struct stat st;

    char *dir = format.myftp_message.message;
    printf("%d\n", format.myftp_message.type);
    if (stat(dir, &st) != 0) {
        fprintf(stderr, "cd: no such file or directory: %s\n", dir);
        return;
    } else {
        switch (st.st_mode & S_IFMT) {
            case S_IFREG:
                fprintf(stderr, "cd: not a directory: %s\n", dir);
                return;
            case S_IFDIR:
                printf("AAA\n");
                chdir(dir);
                return;
            default:
                fprintf(stderr, "cd: no such file or directory: %s\n", dir);
                return;
        }
    }
    return;
}

void my_ls_server(struct myftp_format format)
{
    char *cmd[2];
    cmd[0] = format.myftp_message.message;

    //memcpy(cmd[1], format.myftp_message.message, 64);
    int pid, status;
    if ((pid = fork()) < 0) { //error
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { //child
        execvp("ls", &cmd[0]);
    } else { //parent
        waitpid(pid, &status, 0);
    }
}
