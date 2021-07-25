#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <dirent.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "myftp.h"

extern char pathname[PATHNAME_SIZE];
extern struct myftp_format format;

extern char *server_IP_address;
extern struct in_addr server_net_IP_address;
extern char *client_IP_address;
extern struct in_addr client_net_IP_address;
extern in_port_t myport;
extern in_port_t port;

void my_cd(int argc, char *argv[], int s)
{
    struct stat st;
    int count = 0;

    while (argv[count] != NULL)
        count++;

    if (count == 1) {
        //chdir(ROOT_DIRECTORY);
        return;
    } else if (count == 2) {
        char *dir = argv[1];

        if (stat(dir, &st) != 0) {
            fprintf(stderr, "cd: no such file or directory: %s\n", dir);
            return;
        } else {
            switch (st.st_mode & S_IFMT) {
                case S_IFREG:
                    fprintf(stderr, "cd: not a directory: %s\n", dir);
                    return;
                case S_IFDIR:
                    chdir(dir);
                    return;
                default:
                    fprintf(stderr, "cd: no such file or directory: %s\n", dir);
                    return;
            }
        }
    } else if (count >= 3) {
        fprintf(stderr, "cd: string not in pwd: %s\n", argv[1]);
        return;
    }
    return;
}

void my_pwd_server(int s)
{
    getcwd(pathname, PATHNAME_SIZE);
    format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_OK, 0x00, sizeof(pathname), pathname);
    my_send(s, format);
    printf("%s\n", pathname);
    return;
}

void my_cd_server(int s, struct myftp_format format)
{
    char message[DATASIZE];
    struct stat st;

    char *dir = format.myftp_message.message;
    if (stat(dir, &st) != 0) {
        fprintf(stderr, "cd: no such file or directory: %s\n", dir);
        format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_FILE_ERR, CODE_0, 0, message);
        my_send(s, format);
        return;
    } else {
        switch (st.st_mode & S_IFMT) {
            case S_IFREG:
                fprintf(stderr, "cd: not a directory: %s\n", dir);
                format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_FILE_ERR, CODE_0, 0, message);
                my_send(s, format);
                return;
            case S_IFDIR:
                chdir(dir);
                format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_OK, CODE_0, 0, message);
                my_send(s, format);
                return;
            default:
                fprintf(stderr, "cd: no such file or directory: %s\n", dir);
                format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_FILE_ERR, CODE_0, 0, message);
                my_send(s, format);
                return;
        }
    }
    return;
}

void my_ls_server(int s, struct myftp_format f)
{
    int i;
    int err;
    int argsc;
    char *argsv[BUFLENGTH], lbuf[NARGS];
    char message[DATASIZE];
    DIR *dir;
    struct dirent *dp;
    struct myftp_format format;
    int size = 0;

    setargs(&argsc, argsv, lbuf);
    //strncpy(lbuf, "ls", 3);
    getargs(&argsc, argsv, lbuf);
    argsv[argsc] = f.myftp_message.message;

    format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_OK, CODE_1, 0, message);
    my_send(s, format);

    dir = opendir(argsv[0]);
    for (dp=readdir(dir); dp!=NULL; dp=readdir(dir)) {
        for (i=0; dp->d_name[i]!=0; i++) {
            printf("%c", dp->d_name[i]);
            message[size++] = dp->d_name[i];
            if (size == DATASIZE) {
                printf("\n");
                format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_DATA, CODE_1, 0, message);
                my_send(s, format);
                size = 0;
            }
        }
        message[size++] = '\n';
        if (size == DATASIZE) {
            format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_DATA, CODE_1, 0, message);
            my_send(s, format);
            size = 0;
        }
        printf("\n");
    }
    message[size-1] = '\0';
    printf("%s\n", message);

    format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_DATA, CODE_0, 0, message);
    my_send(s, format);



/*

    int pid, status;
    if ((pid = fork()) < 0) { //error
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { //child
        if ((err = execvp("ls", argsv)) != -1) {
            exit(EXIT_SUCCESS);
        }
        exit(EXIT_FAILURE);
    } else { //parent
        waitpid(pid, &status, 0);
        if (status == 0) {
            format = set_format(0, 0, 0, 0, TYPE_OK, CODE_1, 0, message);
            my_send(s, format);
            memcpy(message, "Hello World", 64);
            format = set_format(0, 0, 0, 0, TYPE_DATA, CODE_0, 0, message);
            my_send(s, format);
        }
        printf("%d\n", status);
    }
*/
}

void my_get_server(int s, struct myftp_format f)
{
    FILE *fp;
    char str;
    int i;
    int size = 0;
    char message[DATASIZE];
    char filename[128];

    for (i=0; f.myftp_message.message[i] != 127; i++) {
        filename[i] = f.myftp_message.message[i];
    }

    if ((fp = fopen("cmd.c", "r")) == NULL) {
        format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_FILE_ERR, CODE_1, size, message);
        my_send(s, format);
        printf("Cannot open file %s\n", f.myftp_message.message);
        return;
    }
    format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_OK, CODE_1, size, message);
    my_send(s, format);
    while((str = fgetc(fp)) != EOF) {
        message[size++] = str;
        printf("%c",str);
        if (size == DATASIZE) {
            printf("\n");
            format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_DATA, CODE_1, size, message);
            my_send(s, format);
            size = 0;
        }
    }

    if (size == DATASIZE) {
        printf("%s\n", message);
        format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_DATA, CODE_1, size, message);
        my_send(s, format);
        printf("\n");
        format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_DATA, CODE_0, sizeof("\0"), "\0");
        my_send(s, format);
    } else {
        message[size-1] = '\n';
        message[size] = '\0';
        printf("%s\n", message);
        format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_DATA, CODE_0, size, message);
        my_send(s, format);
    }

    fclose(fp);
    return;

}

void my_put_server(int s, struct myftp_format f)
{
    FILE *fp;
    char filename[256];
    char message[DATASIZE];
    memcpy(filename, f.myftp_message.message, sizeof(f.myftp_message.message));
    printf("%s\n", filename);
    if ((fp = fopen(filename, "w")) == NULL) {
        printf("Cannot open file %s\n", f.myftp_message.message);
        format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_FILE_ERR, CODE_1, 0, message);
        format = my_recv(s, format);
        return;
    }
    format = set_format(server_net_IP_address.s_addr, client_net_IP_address.s_addr, myport, port, TYPE_OK, CODE_2, 0, message);
    my_send(s, format);
    do {
        format = my_recv(s, format);
        fputs(format.myftp_message.message, fp);
        printf("%s", format.myftp_message.message);
    } while (format.myftp_message.code == CODE_1);
    fclose(fp);
}
