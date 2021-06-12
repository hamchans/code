#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "mysh.h"

int main()
{
    int argc;
    char *argv[BUFLENGTH], lbuf[NARGS];
    extern char pathname[PATHNAME_SIZE];
    memset(pathname, '\0', PATHNAME_SIZE);
    extern struct command_table b_cmd_tbl[];
    int fd;

    while (1) {
        setargs(&argc, argv, lbuf);
        printf("mysh$ ");
        gettoken(&argc, argv, lbuf);

        int pid;
        int i, pl[10], pc = 0;
        int rdrl[10], rdll[10], rdrc = 0, rdlc = 0;
        pl[0] = -1;
        int j = 0, k = 0;
        rdrl[0] = -1, rdll[0] = -1;
        int rdlcnt = 0, rdrcnt = 0;
        int c;
        for (i = 0; argv[i] != NULL; i++) {
            if (strcmp(argv[i], "|") == 0 || strcmp(argv[i], "||") == 0) {
                pl[++pc] = i;
                argv[i] = NULL;
            } else if (strcmp(argv[i], "<") == 0 || strcmp(argv[i], "<<") == 0) {
                rdll[rdlc++] = i;
                argv[i] = NULL;
            } else if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0) {
                rdrl[rdrc++] = i;
                argv[i] = NULL;
            }
        }

        int pfd[9][2];
        if (pc == 0) { //no pipe
            if (rdrc == 0 && rdlc == 0) { //no redirect
                struct command_table *p;
                for (p = b_cmd_tbl; p->cmd; p++) {
                    if (strcmp(argv[0], p->cmd) == 0) {
                        (*p->func)(argc, argv);
                        break;
                    }
                }
                if (p->cmd == NULL) {
                    if (fork() == 0) {
                        execvp(argv[0], argv);
                        exit(EXIT_SUCCESS);
                    }
                    else {
                        int status;
                        wait(&status);
                    }
                }
            } else if (rdlc != 0 && rdrc == 0) { //<<
                struct command_table *p;
                for (p = b_cmd_tbl; p->cmd; p++) {
                    if (strcmp(argv[0], p->cmd) == 0) {
                        if ((fd = open(argv[rdll[rdlcnt]+1], O_RDONLY, 0644)) == -1) {
                            fprintf(stderr, "bash: no such file or directory: %s\n", argv[rdll[rdlcnt]+1]);
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd, 0);
                        close(fd);
                        printf("<<\n");
                        (*p->func)(argc, argv);
                        break;
                    }
                }
                if (p->cmd == NULL) {
                    if (fork() == 0) { //child
                        if ((fd = open(argv[rdll[rdlcnt]+1], O_RDONLY, 0644)) == -1) {
                            fprintf(stderr, "bash: no such file or directory: %s\n", argv[rdll[rdlcnt]+1]);
                            exit(EXIT_FAILURE);
                        }

                        dup2(fd, 0);
                        close(fd);
                        printf("<<\n");
                        execvp(argv[0], argv);
                        exit(EXIT_SUCCESS);
                    }
                    else {
                        int status;
                        wait(&status);
                    }
                }
            } else if (rdrc != 0 && rdlc == 0) { //>>
                struct command_table *p;
                for (p = b_cmd_tbl; p->cmd; p++) {
                    if (strcmp(argv[0], p->cmd) == 0) {
                        fd = open(argv[rdrl[rdrcnt]+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
                        dup2(fd, 1);
                        close(fd);
                        printf(">>\n");
                        (*p->func)(argc, argv);
                        break;
                    }
                }
                if (p->cmd == NULL) {
                    if (fork() == 0) { //child
                        fd = open(argv[rdrl[rdrcnt]+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
                        dup2(fd, 1);
                        close(fd);
                        printf(">>\n");
                        execvp(argv[0], argv);
                        exit(EXIT_SUCCESS);
                    }
                    else {
                        int status;
                        wait(&status);
                    }
                }
            } else {
                fprintf(stderr, "redirect is 1\n");
            }
        }

        for (i=0; i<pc+1 && pc!=0; i++) { //exist pipe
            printf("%d, %d\n", i, pc);
            //if (i != pc) //if not last conmand then create pipe
                pipe(pfd[i]);

            //execvp(argv[pl[i]+1], argv + pl[i] + 1);

            if (rdll[rdlcnt] < pl[i] && rdll[rdlcnt] != -1) { //<<
                //<<
                struct command_table *p;
                for (p = b_cmd_tbl; p->cmd; p++) {
                    if (strcmp(argv[pl[i]+1], p->cmd) == 0) {
                        if ((fd = open(argv[rdll[rdlcnt]+1], O_RDONLY, 0644)) == -1) {
                            fprintf(stderr, "bash: no such file or directory: %s\n", argv[rdll[rdlcnt]+1]);
                            exit(EXIT_FAILURE);
                        }
                        dup2(fd, 0);
                        close(fd);
                        printf("<<\n");
                        (*p->func)(argc, argv + pl[i] + 1);
                        break;
                    }
                }
                if (p->cmd == NULL) {
                    if ((pid = fork()) < 0) { //error
                        perror("fork");
                        exit(1);
                    } else if (pid == 0) { //child process
                        if (i == 0) { //first command
                            dup2(pfd[i][1], 1);
                            close(pfd[i][0]);
                            close(pfd[i][1]);
                        } else if (i == pc) { //last command
                            dup2(pfd[i-1][0], 0);
                            close(pfd[i-1][0]);
                            close(pfd[i-1][1]);
                        } else { //neither first command nor last one
                            dup2(pfd[i-1][0], 0);
                            dup2(pfd[i][1], 1);
                            close(pfd[i-1][0]);
                            close(pfd[i-1][1]);
                            close(pfd[i][0]);
                            close(pfd[i][1]);
                        }

                        if ((fd = open(argv[rdll[rdlcnt]+1], O_RDONLY, 0644)) == -1) {
                            fprintf(stderr, "bash: no such file or directory: %s\n", argv[rdll[rdlcnt]+1]);
                            exit(EXIT_FAILURE);
                        }

                        dup2(fd, 0);
                        close(fd);
                        printf("<<\n");

                        execvp(argv[pl[i]+1], argv + pl[i] + 1);
                        exit(EXIT_SUCCESS);
                    } else { //parent process
                        close(pfd[i-1][0]);
                        close(pfd[i-1][1]);
                        int status;
                        wait(&status);
                    }
                }
                rdlcnt++;
            } else if (rdrl[rdrcnt] < pl[i] && rdrl[rdrcnt] != -1) { //>>
                //>>
                struct command_table *p;
                for (p = b_cmd_tbl; p->cmd; p++) {
                    if (strcmp(argv[pl[i]+1], p->cmd) == 0) {
                        fd = open(argv[rdrl[rdrcnt]+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
                        dup2(fd, 1);
                        close(fd);
                        printf(">>\n");
                        (*p->func)(argc, argv + pl[i] + 1);
                        break;
                    }
                }
                if (p->cmd == NULL) {

                    if ((pid = fork()) < 0) { //error
                        perror("fork");
                        exit(1);
                    } else if (pid == 0) { //child process
                        if (i == 0) { //first command
                            dup2(pfd[i][1], 1);
                            close(pfd[i][0]);
                            close(pfd[i][1]);
                        } else if (i == pc) { //last command
                            dup2(pfd[i-1][0], 0);
                            close(pfd[i-1][0]);
                            close(pfd[i-1][1]);
                        } else { //neither first command nor last one
                            dup2(pfd[i-1][0], 0);
                            dup2(pfd[i][1], 1);
                            close(pfd[i-1][0]);
                            close(pfd[i-1][1]);
                            close(pfd[i][0]);
                            close(pfd[i][1]);
                        }

                        fd = open(argv[rdrl[rdrcnt]+1], O_WRONLY|O_CREAT|O_TRUNC, 0644);
                        dup2(fd, 1);
                        close(fd);
                        printf(">>\n");

                        execvp(argv[pl[i]+1], argv + pl[i] + 1);
                        exit(EXIT_SUCCESS);
                    } else { //parent process
                        close(pfd[i-1][0]);
                        close(pfd[i-1][1]);
                        int status;
                        wait(&status);
                    }
                }
            } else { //no redirect
                struct command_table *p;
                for (p = b_cmd_tbl; p->cmd; p++) {
                    if (strcmp(argv[pl[i]+1], p->cmd) == 0) {
                        (*p->func)(argc, argv + pl[i] + 1);
                        break;
                    }
                }
                if (p->cmd == NULL) {
                    printf("BBB\n");

                    if ((pid = fork()) < 0) { //error
                        perror("fork");
                        exit(1);
                    } else if (pid == 0) { //child process
                        if (i == 0) { //first command
                            dup2(pfd[i][1], 1);
                            close(pfd[i][0]);
                            close(pfd[i][1]);
                        } else if (i == pc) { //last command
                            dup2(pfd[i-1][0], 0);
                            close(pfd[i-1][0]);
                            close(pfd[i-1][1]);
                        } else { //neither first command nor last one
                            dup2(pfd[i-1][0], 0);
                            dup2(pfd[i][1], 1);
                            close(pfd[i-1][0]);
                            close(pfd[i-1][1]);
                            close(pfd[i][0]);
                            close(pfd[i][1]);
                        }

                        execvp(argv[pl[i]+1], argv + pl[i] + 1);
                        exit(EXIT_SUCCESS);
                    } else { //parent process
                        close(pfd[i-1][0]);
                        close(pfd[i-1][1]);
                        int status;
                        wait(&status);
                    }
                }
            }

        }
        int status;

        for (i=0; i<pc+1; i++)
            wait(&status);
    }

    return 0;
}
