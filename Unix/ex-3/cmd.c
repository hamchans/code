#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include "mysh.h"

extern char pathname[PATHNAME_SIZE];

struct command_table b_cmd_tbl[] = {
    {"cd", my_cd},
    {"pwd", my_pwd},
    {"exit", my_exit},
    {NULL, NULL}
};

void my_cd(int argc, char *argv[])
{
    int count = 0;
    while (argv[count] != NULL)
        count++;

    struct stat st;

    if (count == 1) {
        chdir(ROOT_DIRECTORY);
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

void my_pwd(int argc, char *argv[])
{
    //execvp("/bin/pwd", argv);
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

void my_exit(int argc, char *argv[])
{
    exit(0);
    return;
}

void exec_b_cmd(int argc, char *argv[])
{
    struct command_table *p;
    for (p = b_cmd_tbl; p->cmd; p++) {
        if (strcmp(argv[0], p->cmd) == 0) {
            (*p->func)(argc, argv);
            break;
        }
    }
    if (p->cmd == NULL)
        execvp(argv[0], argv);
        //fprintf(stderr, "mysh: command not found: %s\n", argv[0]);
}
