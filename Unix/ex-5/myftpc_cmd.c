#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#include "myftp.h"

extern char pathname[PATHNAME_SIZE];
extern in_port_t port;
extern char *server_IP_address;
extern struct in_addr server_net_IP_address;
extern char *client_IP_address;
extern struct in_addr client_net_IP_address;

struct command_table cmd_tbl[] = {
    {"quit", quit_client},
    {"pwd", my_pwd},
    {"cd", my_cd},
    {"dir", my_dir},
    {"lpwd", my_lpwd},
    {"lcd", my_lcd},
    {"ldir", my_ldir},
    {"get", my_get},
    {"put", my_put},
    {"help", my_help},
    {NULL, NULL}
};

void init_client()
{
    printf("myFTP%% ");
}

void quit_client(int argsc, char *argsv[], int s)
{
    char message[DATASIZE];
    extern struct myftp_format format;
    format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_QUIT, 0, 0, message);
    my_send(s, format);
    close(s);
    exit(EXIT_SUCCESS);
}

void my_pwd(int argsc, char *argsv[], int s)
{
    char message[DATASIZE];
    extern struct myftp_format format;
    format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_PWD, 0, 0, message);
    my_send(s, format);
    format = my_recv(s, format);
    printf("%s\n", format.myftp_message.message);
}

void my_cd(int argsc, char *argsv[], int s)
{
    char message[DATASIZE];
    extern struct myftp_format format;

    //struct stat st;
    //int count = 0;

    //while (argsv[count] != NULL)
    //    count++;

    if (argsc == 1) {
        memcpy(message, ROOT_DIRECTORY, DATASIZE);
    } else if (argsc == 2) {
        memcpy(message, argsv[1], 64);
    } else if (argsc >= 3) {
        fprintf(stderr, "cd: string not in pwd: %s\n", argsv[1]);
        return;
    }
    format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_CWD, 0, 0, message);
    printf("%s\n", format.myftp_message.message);
    my_send(s, format);
    format = my_recv(s, format);
}

void my_dir(int argsc, char *argsv[], int s)
{
    char message[DATASIZE];
    extern struct myftp_format format;
    if (argsc == 1) {
        memcpy(message, ".", DATASIZE);
    } else if (argsc == 2) {
        memcpy(message, argsv[1], DATASIZE);
    }
    format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_LIST, 0, 0, message);
    my_send(s, format);
    format = my_recv(s, format);
    printf("%s\n", format.myftp_message.message);
    do {
        format = my_recv(s, format);
        printf("%s", format.myftp_message.message);
    } while (format.myftp_message.code == CODE_1);
    printf("\n");
}

void my_lpwd(int argsc, char *argsv[], int s)
{
    getcwd(pathname, PATHNAME_SIZE);
    printf("%s\n", pathname);
    return;
}

void my_lcd(int argsc, char *argsv[], int s)
{
    struct stat st;
    int count = 0;

    while (argsv[count] != NULL)
        count++;

    if (count == 1) {
        chdir(ROOT_DIRECTORY);
        return;
    } else if (count == 2) {
        char *dir = argsv[1];

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
        fprintf(stderr, "cd: string not in pwd: %s\n", argsv[1]);
        return;
    }
    return;
}

void my_ldir(int argsc, char *argsv[], int s)
{
    int pid, status;
    if ((pid = fork()) < 0) { //error
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { //child
        execvp("ls", &argsv[0]);
    } else { //parent
        waitpid(pid, &status, 0);
    }
}

void my_get(int argsc, char *argsv[], int s)
{
	FILE *fp;
    char filename[256];

    int i;
    char message[DATASIZE];
    extern struct myftp_format format;
    if (argsc == 1) {
        fprintf(stderr, "Please input in correct sysntax\n");
        return;
    } else if (argsc == 2 || argsc == 3) {
        memcpy(filename, argsv[argsc - 1], sizeof(argsv[2]));

        for (i=0; argsv[1][i] != '\0'; i++) {
            message[i] = argsv[1][i];
        }
        format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_RETR, 0, sizeof(argsv[1]), message);
        my_send(s, format);
        do {
            format = my_recv(s, format);
            if (format.myftp_message.type == TYPE_OK) {
                if ((fp = fopen(filename, "w")) == NULL) {
                    printf("Cannot open file %s\n", filename);
                    return;
                }
                fputs(format.myftp_message.message, fp);
                printf("%s", format.myftp_message.message);
            } else if (format.myftp_message.type == TYPE_DATA) {
                fputs(format.myftp_message.message, fp);
                printf("%s", format.myftp_message.message);
            } else {
                fprintf(stderr, "Cannot open file %s\n", message);
                return;
            }
        } while (format.myftp_message.code == CODE_1);
        fclose(fp);
        printf("\n");
    } else {
        fprintf(stderr, "Please input in correct sysntax\n");
        return;
    }
}

void my_put(int argsc, char *argsv[], int s)
{
    int i;
    FILE *fp;
    char filename[256];
    char str;
    char message[DATASIZE];
    int size = 0;
    extern struct myftp_format format;
    if (argsc == 1) {
        fprintf(stderr, "Please input in correct sysntax\n");
        return;
    } else if (argsc == 2 || argsc == 3) {

        for (i=0; argsv[1][i] != 0; i++) {
            filename[i] = argsv[1][i];
        }

        if ((fp = fopen(filename, "r")) == NULL) {
            printf("Cannot open file %s\n", filename);
            return;
        }
        memcpy(filename, argsv[argsc - 1], sizeof(*argsv[argsc - 1]));
        format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_STOR, 0, 0, filename);
        my_send(s, format);
        format = my_recv(s, format);
        if (format.myftp_message.type == TYPE_OK) {
            while((str = fgetc(fp)) != EOF) {
                message[size++] = str;
                printf("%c", str);
                if (size == DATASIZE) {
                    printf("\n");
                    format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_DATA, CODE_1, size, message);
                    my_send(s, format);
                    size = 0;
                }
            }
            if (size == DATASIZE) {
                printf("%s\n", message);
                format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_DATA, CODE_1, size, message);
                my_send(s, format);
                printf("\n");
                format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_DATA, CODE_0, sizeof("\0"), "\0");
                my_send(s, format);
            } else {
                message[size-1] = '\n';
                message[size] = '\0';
                printf("%s\n", message);
                format = set_format(client_net_IP_address.s_addr, server_net_IP_address.s_addr, 0, port, TYPE_DATA, CODE_0, size, message);
                my_send(s, format);
            }

            fclose(fp);
            printf("\n");
        } else {
            fprintf(stderr, "Cannot open file %s\n", filename);
            return;
        }
    } else {
        fprintf(stderr, "Please input in correct sysntax\n");
        return;
    }
}

void my_help(int argsc, char *argsv[], int s)
{
    printf("quit:\t\t\t quit myftpc\n");
    printf("pwd:\t\t\t show current directory on server\n");
    printf("cd path:\t\t move from current directory to path directory on server\n");
    printf("dir [path]:\t\t get file information exisitng on server\n");
    printf("lpwd:\t\t\t show current directory on client\n");
    printf("lcd path:\t\t move from current directory to path directory on client\n");
    printf("ldir [path]:\t\t get file information exisitng on client\n");
    printf("get path1 [path2]:\t transfer files from server to client\n");
    printf("put path1 [path2]:\t transfer files from client to server\n");
    printf("help:\t\t\t show help\n");
}
