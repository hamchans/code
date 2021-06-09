#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include "mysh.h"

int main()
{
    int argc;
    char *argv[BUFLENGTH], lbuf[NARGS];
    extern char pathname[PATHNAME_SIZE];
    memset(pathname, '\0', PATHNAME_SIZE);
    extern struct command_table b_cmd_tbl[];

    while (1) {
        setargs(&argc, argv, lbuf);
        printf("mysh$ ");
        gettoken(&argc, argv, lbuf);

        int i, pl[10], pc = 0;
        int rdrl[10], rdll[10], rdrc = 0, rdlc = 0;
        pl[0] = -1;
        int j = 0, k = 0;
        rdrl[0] = -1, rdll[0] = -1;
        for (i = 0; argv[i] != NULL; i++) {
            if (strcmp(argv[i], "|") == 0 || strcmp(argv[i], "||") == 0) {
                pl[++pc] = i;
                argv[i] = NULL;
            } else if (strcmp(argv[i], "<") == 0 || strcmp(argv[i], "<<") == 0) {
                rdll[++rdlc] = i;
                argv[i] = NULL;
            } else if (strcmp(argv[i], ">") == 0 || strcmp(argv[i], ">>") == 0) {
                rdrl[++rdrc] = i;
                argv[i] = NULL;
            }
        }

        int pfd[9][2];
        if (pc == 0) { //no pipe
            if (rdrc == 0 && rdlc == 0) {
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
            } else {
                for (i=0; i<rdrc+rdlc; i++) {
                    if (rdrl[j] == -1) {
                        //<<
                    } else if (rdll[k] == -1) {
                        //>>
                    } else {
                        if (rdrl[j] > rdll[k]) {
                            //<<
                        } else {
                            //>>
                        }
                    }
                }
            }
        }

        for (i=0; i<pc+1 && pc!=0; i++) {
            if (i != pc)
                pipe(pfd[i]);

            struct command_table *p;
            for (p = b_cmd_tbl; p->cmd; p++) {
                if (strcmp(argv[0], p->cmd) == 0) {
                    (*p->func)(argc, argv);
                    break;
                }
            }
            if (p->cmd == NULL) {
                if (fork() == 0) {
                    if (i == 0) {
                        dup2(pfd[i][1], 1);
                        close(pfd[i][0]);
                        close(pfd[i][1]);
                    } else if (i == pc) {
                        dup2(pfd[i-1][0], 0);
                        close(pfd[i-1][0]);
                        close(pfd[i-1][1]);
                    } else {
                        dup2(pfd[i-1][0], 0);
                        dup2(pfd[i][1], 1);
                        close(pfd[i-1][0]);
                        close(pfd[i-1][1]);
                        close(pfd[i][0]);
                        close(pfd[i][1]);
                    }

                    execvp(argv[pl[i]+1], argv + pl[i] + 1);
                    exit(EXIT_SUCCESS);
                }
                else if (i > 0) {
                    close(pfd[i-1][0]);
                    close(pfd[i-1][1]);
                }
            }

        }
        int status;

        for (i=0; i<pc+1; i++)
            wait(&status);
    }

    return 0;
}
