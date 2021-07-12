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
extern char **environ;

void proc_exit()
{
    int wstat;
    pid_t pid;
    int n;

    while (1) {
        pid = wait3(&wstat, WNOHANG, NULL);
        if (pid == 0 || pid == -1)
            return;
        }
}

struct command_table b_cmd_tbl[] = {
    {"cd", my_cd},
    {"pwd", my_pwd},
    {"exit", my_exit},
    {NULL, NULL}
};

void my_cd(int argc, char *argv[])
{
    struct stat st;
    int count = 0;

    while (argv[count] != NULL)
        count++;

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
    exit(EXIT_SUCCESS);
}

void my_dup(int i, int pipe_count, int pfd[9][2])
{
    if (i == 0) {
        dup2(pfd[i][1], 1);
        close(pfd[i][0]);
        close(pfd[i][1]);
    } else if (i == pipe_count) {
        dup2(pfd[i - 1][0], 0);
        close(pfd[i - 1][0]);
        close(pfd[i - 1][1]);
    } else {
        dup2(pfd[i - 1][0], 0);
        dup2(pfd[i][1], 1);
        close(pfd[i - 1][0]);
        close(pfd[i - 1][1]);
        close(pfd[i][0]);
        close(pfd[i][1]);
    }
    return;
}

void my_exec(int argc, char *argv[], int *fd, int pipe_locate_num, int pipe_count, int pfd[9][2], int i, int redirect, int redirect_location[10], int rd_num, int bg)
{
    extern int array[128];
    extern char *cmd_path[CMD_PATH_LEN + 1];
    char command[128];
    int wstatus, num, pid, rc = 0;
    struct sigaction act;

    cmd_path[0] = "/usr/bin/";

    memset(&act, 0, sizeof(act));
    act.sa_handler = ignore;
    act.sa_flags = SA_RESETHAND;

    rc = sigaction(SIGINT, &act, NULL);
    if(rc < 0) {
        fprintf(stderr, "Error: sigaction() %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    struct command_table *p;
    for (p = b_cmd_tbl; p->cmd; p++) {
        if (strcmp(argv[pipe_locate_num + 1], p->cmd) == 0) { //if built-in command
            my_redirect(argc, argv, fd, redirect, redirect_location, rd_num);
            (*p->func)(argc, argv + pipe_locate_num + 1);
            break;
        }
    }

    if (p->cmd == NULL) { //if not built-in command
        signal(SIGCHLD, proc_exit);
        if ((pid = fork()) < 0) { //error
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { //child
            my_dup(i, pipe_count, pfd);
            setpgid(0, 0);
            my_redirect(argc, argv, fd, redirect, redirect_location, rd_num);

            //prepare execve
            memcpy(command, cmd_path[0], CMD_PATH_LEN);
            memcpy(command + CMD_PATH_LEN, argv[pipe_locate_num + 1], 10);

            //execvp(argv[pipe_locate_num + 1], argv + pipe_locate_num + 1);


            if ((num = execve(command, argv + pipe_locate_num + 1, environ)) == 0)
                exit(EXIT_SUCCESS);
            else { // if execve is failed
                if ((num = execvp(argv[pipe_locate_num + 1], argv + pipe_locate_num + 1)) == 0)
                    exit(EXIT_SUCCESS);
                else {
                    fprintf(stderr, "%s: Command not found\n", argv[pipe_locate_num + 1]);
                    exit(EXIT_FAILURE);
                }
            }

        } else { //parent
            if (!(bg > 0)) {
                if (i == 0) {
                    int status;
                    waitpid(pid, &status, 0);
                } else if (i > 0) {
                    close(pfd[i - 1][0]);
                    close(pfd[i - 1][1]);
                    int status;
                    waitpid(pid, &status, 0);
                }
            }
        }
    }
    return;
}

void my_redirect(int argc, char *argv[], int *fd, int redirect, int redirect_location[10], int rd_num)
{
    if (redirect == 1) { //>
        *fd = open(argv[redirect_location[rd_num]+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
        dup2(*fd, 1);
        close(*fd);
    } else if (redirect == 2) { //>>
        *fd = open(argv[redirect_location[rd_num]+1], O_RDWR|O_CREAT|O_APPEND, 0644);
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

    }
    return;
}

void ignore() {}
