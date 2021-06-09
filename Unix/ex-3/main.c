//61820353 Kohki Yamada

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "mysh.h"

int main()
{
    int i, pc = 0;

    int pid;
    int *stat;

    int pfd[9][2]; //9 pipes
    int pl[10]; //pipe location
    pl[0] = -1;

    int argc;
    char *argv[BUFLENGTH], lbuf[NARGS];
    extern char pathname[PATHNAME_SIZE];
    memset(pathname, '\0', PATHNAME_SIZE);

    while (1) {
        setargs(&argc, argv, lbuf);
        printf("mysh$ ");
        gettoken(&argc, argv, lbuf);

        for (i = 0; argv[i] != NULL; i++) {
            if (strcmp(argv[i], "|") == 0 || strcmp(argv[i], "||") == 0) {
                //pc++;
                pl[++pc] = i;
                argv[i] = NULL;
            }
        }

        for (i=0; i<argc; i++) {
            printf("%s\n", argv[i]);
        }
        for (i=0; i<pc+1; i++) {
            printf("%d\n", pl[i]);
        }

        if (pc == 0) {
            if (fork() == 0) {
                execvp(argv[0], argv);
                exit(0);
            }
            else {
                int status;
                wait(&status);
            }
        } else {
            for (i=0; i<pc+1 && pc!=0; i++) {
                if (i != pc)
                    pipe(pfd[i]);
                if ((pid = fork()) < 0) { //error
                    perror("fork");
                    exit(1);
                } else if (pid == 0) { //child process
                    //execvp(argv[0], argv);
                    if (i == 0) { //first command
                        printf("first\n");
                        dup2(pfd[i][1], 1);
                        close(pfd[i][0]);
                        close(pfd[i][1]);
                    } else if (i == pc) { //last command
                        printf("last\n");
                        dup2(pfd[i-1][0], 0);
                        close(pfd[i-1][0]);
                        close(pfd[i-1][1]);
                    } else { //neither first command nor last one
                        printf("not first last\n");
                        dup2(pfd[i-1][0], 0);
                        dup2(pfd[i][1], 1);
                        close(pfd[i-1][0]); close(pfd[i-1][1]);
                        close(pfd[i][0]); close(pfd[i][1]);
                    }
                    printf("execvp\n");
                    execvp(argv[pl[i]+1], argv + pl[i] + 1);
                    //exit(0);

                    //exec_b_cmd(argc, argv);
                    exit(EXIT_SUCCESS);
                } else { //parent process
                    close(pfd[i-1][0]);
                    close(pfd[i-1][1]);
                }
                int status;

                for (i=0; i<pc+1; i++) {
                    wait(&status);
                }
            }
        }
    }
    return 0;
}
