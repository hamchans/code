#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
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

void my_dup(int i, int pipe_count, int pfd[9][2])
{
    if (i == 0) {
        dup2(pfd[i][1], 1);
        close(pfd[i][0]); close(pfd[i][1]);
    } else if (i == pipe_count) {
        dup2(pfd[i - 1][0], 0);
        close(pfd[i - 1][0]); close(pfd[i - 1][1]);
    } else {
        dup2(pfd[i - 1][0], 0);
        dup2(pfd[i][1], 1);
        close(pfd[i - 1][0]); close(pfd[i - 1][1]);
        close(pfd[i][0]); close(pfd[i][1]);
    }
}

void my_exec(int argc, char *argv[], int *fd, int pipe_locate_num, int pfd[9][2], int i, int redirect, int redirect_location[10], int rd_num)
{
    int rc = 0;
    struct sigaction act;

    memset(&act, 0, sizeof(act));
    act.sa_handler = exit_process;
    act.sa_flags = SA_RESETHAND;

    rc = sigaction(SIGINT, &act, NULL);
    if(rc < 0) {
        printf("Error: sigaction() %s\n", strerror(errno));
        exit(1);
    }

    int pid;
    struct command_table *p;
    for (p = b_cmd_tbl; p->cmd; p++) {
        if (strcmp(argv[pipe_locate_num + 1], p->cmd) == 0) { //if not built-in command
            my_redirect(argc, argv, fd, redirect, redirect_location, rd_num);
            (*p->func)(argc, argv + pipe_locate_num + 1);
            break;
        }
    }
    if (p->cmd == NULL) { //if built-in command
        if ((pid = fork()) < 0) { //error
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { //child
            my_redirect(argc, argv, fd, redirect, redirect_location, rd_num);

            execvp(argv[pipe_locate_num + 1], argv + pipe_locate_num + 1);

            exit(EXIT_SUCCESS);
        } else { //parent
            if (i == 0) {
                int status;
                wait(&status);
            } else if (i > 0) {
                close(pfd[i - 1][0]); close(pfd[i - 1][1]);
            }
        }
    }
}

void my_redirect(int argc, char *argv[], int *fd, int redirect, int redirect_location[10], int rd_num)
{
    if (redirect == 1) { //>
        *fd = open(argv[redirect_location[rd_num]+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
        dup2(*fd, 1);
        close(*fd);
    } else if (redirect == 2) { //>>
        *fd = open(argv[redirect_location[rd_num]+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
        dup2(*fd, 1);
        close(*fd);
    } else if (redirect == 3) { //<
        if ((*fd = open(argv[redirect_location[rd_num]+1], O_RDONLY, 0644)) == -1) {
            fprintf(stderr, "bashA: no such file or directory: %s\n", argv[redirect_location[rd_num]+1]);
            exit(EXIT_FAILURE);
        }
        dup2(*fd, 0);
        close(*fd);
    } else if (redirect == 4) { //<<
        if ((*fd = open(argv[redirect_location[rd_num]+1], O_RDONLY, 0644)) == -1) {
            fprintf(stderr, "bashB: no such file or directory: %s\n", argv[redirect_location[rd_num]+1]);
            exit(EXIT_FAILURE);
        }
        dup2(*fd, 0);
        close(*fd);
    } else if (redirect == 0) {
        ;
    }
}

void exit_process()
{
    fprintf(stderr, "Exit normally.\n");
    exit(0);
}
