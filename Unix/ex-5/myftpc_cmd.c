#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include "myftp.h"

extern char pathname[PATHNAME_SIZE];

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
    char message[64];
    extern struct myftp_format format;
    memcpy(message, "Hello", 64);
    format = set_format(0, 0, 0, 0, TYPE_QUIT, 0, 0, message);
    my_send(s, format);
    close(s);
    exit(EXIT_SUCCESS);
}

void my_pwd(int argsc, char *argsv[], int s)
{
    char message[64];
    extern struct myftp_format format;
    memcpy(message, "Hello", 64);
    format = set_format(0, 0, 0, 0, TYPE_PWD, 0, 0, message);
    my_send(s, format);
}

void my_cd(int argsc, char *argsv[], int s)
{
    char message[64];
    extern struct myftp_format format;

    struct stat st;
    int count = 0;

    while (argsv[count] != NULL)
        count++;

    if (count == 1) {
        memcpy(message, ROOT_DIRECTORY, 64);
    } else if (count == 2) {
        memcpy(message, argsv[1], 64);
    } else if (count >= 3) {
        fprintf(stderr, "cd: string not in pwd: %s\n", argsv[1]);
        return;
    }
    format = set_format(0, 0, 0, 0, TYPE_CWD, 0, 0, message);
    printf("%s\n", format.myftp_message.message);
    my_send(s, format);
}

void my_dir(int argsc, char *argsv[], int s)
{
    char message[64];
    extern struct myftp_format format;
    if (argsc == 1) {
        memcpy(message, ".", 64);
    } else if (argsc == 2) {
        memcpy(message, argsv[1], 64);
    }
    format = set_format(0, 0, 0, 0, TYPE_LIST, 0, 0, message);
    my_send(s, format);
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
        execvp("ls", argsv);
    } else { //parent
        waitpid(pid, &status, 0);
    }
}

void my_get(int argsc, char *argsv[], int s)
{
    char message[64];
    extern struct myftp_format format;
    memcpy(message, "Hello", 64);
    format = set_format(0, 0, 0, 0, TYPE_RETR, 0, 0, message);
    my_send(s, format);
}

void my_put(int argsc, char *argsv[], int s)
{
    char message[64];
    extern struct myftp_format format;
    memcpy(message, "Hello", 64);
    format = set_format(0, 0, 0, 0, TYPE_STOR, 0, 0, message);
    my_send(s, format);
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
