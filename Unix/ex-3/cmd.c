#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "mysh.h"
extern char pathname[PATHNAME_SIZE];
struct command_table cmd_tbl[] = {
    {"cd", my_cd},
    {"pwd", my_pwd},
    {"ls", my_ls},
    {"exit", my_exit},
    {NULL, NULL}
};

void my_cd(int argc, char *argv[])
{
    struct stat st;

    if (argc == 1) {
        chdir(ROOT_DIRECTORY);
        return;
    } else if (argc == 2) {
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
    } else if (argc >= 3) {
        fprintf(stderr, "cd: string not in pwd: %s\n", argv[1]);
        return;
    }
    return;
}

void my_pwd(int argc, char *argv[])
{
    if (argc == 1) {
        getcwd(pathname, PATHNAME_SIZE);
        printf("%s\n", pathname);
        return;
    } else if (argc >= 2) {
        fprintf(stderr, "pwd: too many arguments\n");
        return;
    }
    return;
}

void my_ls(int argc, char *argv[]) //directory only
{
    int i;
    DIR *dp;
    struct dirent *dir;
    char *pathname[PATHNAME_SIZE];
    int pathc = 0;

    if (argc == 1)
        pathname[pathc++] = ".";
    else if (argc >= 2) {
        if (argv[1][0] == '-') {
            if (argc == 2)
                pathname[pathc++] = ".";
            for (i=2; i<argc; i++)
                pathname[pathc++] = argv[i];
        } else {
            for (i=1; i<argc; i++)
                pathname[pathc++] = argv[i];
        }
    }

    for (i=0; i<pathc; i++) {
        if ((dp = opendir(pathname[i])) == NULL ) {
            fprintf(stderr, "ls: %s: No such file\n", pathname[i]);
            return;
        }

        if (pathc != 1)
            printf("%s:\n", pathname[i]);

        while ((dir = readdir(dp)) != NULL) {
            if (dir->d_ino == 0)
                continue;

            (void)printf("%s\n", dir->d_name);
        }
        if (i != pathc - 1)
            printf("\n");

        (void)closedir(dp);
    }
    return;
}

void my_exit(int argc, char *argv[])
{
    exit(0);
    return;
}

void exec_cmd(int argc, char *argv[])
{
    struct command_table *p;
    for (p = cmd_tbl; p->cmd; p++) {
        if (strcmp(argv[0], p->cmd) == 0) {
            (*p->func)(argc, argv);
            break;
        }
    }
    if (p->cmd == NULL)
        fprintf(stderr, "mysh: command not found: %s\n", argv[0]);
}
