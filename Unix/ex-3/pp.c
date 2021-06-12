#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>
#include "mysh.h"

int main()
{

    int argc;
    char *argv[BUFLENGTH], lbuf[NARGS];
    extern char pathname[PATHNAME_SIZE];
    memset(pathname, '\0', PATHNAME_SIZE);
    extern struct command_table b_cmd_tbl[];
    void *sig;

    while (1) {
        int fd;
        int pid;
        int pfd[9][2];
        int i, pipe_locate[10], pipe_count = 0;
        int left_count1 = 0, left_count2 = 0, right_count1 = 0, right_count2 = 0;
        int left1[10], left2[10], right1[10], right2[10];
        pipe_locate[0] = -1;
        for (i=0; i<10; i++) {
            left1[i] = -1;
            left2[i] = -1;
            right1[i] = -1;
            right2[i] = -1;
        }
        //left1[0] = -1, left2[0] = -1, right1[0] = -1, right2[0] = -1;
        int l1 = 0, l2 = 0, r1 = 0, r2 = 0;

        //struct sigaction *act, *oldact;

        setargs(&argc, argv, lbuf);
        printf("mysh$ ");

        //sig = sigaction(SIGINT, act, oldact);

        int rc = 0;
        struct sigaction act;

        memset(&act, 0, sizeof(act));
        act.sa_handler = exit_process;
        act.sa_flags = SA_RESETHAND;
/*
        rc = sigaction(SIGINT, &act, NULL);
        if(rc < 0) {
            printf("Error: sigaction() %s\n", strerror(errno));
            return(-1);
        }
*/
/*
        sig = signal(SIGINT, SIG_IGN);
        if( SIG_ERR == sig ){
            exit(-1);
        }
*/
        gettoken(&argc, argv, lbuf);

        for (i = 0; argv[i] != NULL; i++) {
            if (strcmp(argv[i], "|") == 0 || strcmp(argv[i], "||") == 0) {
                pipe_locate[++pipe_count] = i;
                argv[i] = NULL;
            } else if (strcmp(argv[i], "<") == 0) {
                left1[left_count1++] = i;
                argv[i] = NULL;
            } else if (strcmp(argv[i], "<<") == 0) {
                left2[left_count2++] = i;
                argv[i] = NULL;
            } else if (strcmp(argv[i], ">") == 0) {
                right1[right_count1++] = i;
                argv[i] = NULL;
            } else if (strcmp(argv[i], ">>") == 0) {
                right2[right_count2++] = i;
                argv[i] = NULL;
            }
        }

        if (pipe_count == 0) { //no pipe
            if (left1[0] != -1) {
                if (right1[0] != -1) { //< >
                    if (left1[0] > right1[0]) { //> <
                        //my_redirect(argc, argv, 1, right1, right_count1);
                    } else { //< >

                    }
                } else if (right2[0] != -1) { //< >>

                } else { //<
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, 0, 3, left1, l1++);
                }
            } else if (left2[0] != -1) {
                if (right1[0] != -1) { //<< >

                } else if (right2[0] != -1) { // << >>

                } else { //<<
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, 0, 4, left2, l2++);
                }
            } else {
                if (right1[0] != -1) { //>
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, 0, 1, right1, r1++);
                } else if (right2[0] != -1) { // >>
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, 0, 2, right2, r2++);
                } else { //no redirect
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, 0, 0, right1, 0);
                }
            }
/*
            struct command_table *p;
            for (p = b_cmd_tbl; p->cmd; p++) {
                if (strcmp(argv[0], p->cmd) == 0) { //if not built-in command
                    (*p->func)(argc, argv);
                    break;
                }
            }
            if (p->cmd == NULL) { //if built-in command
                if ((pid = fork()) < 0) { //error
                    perror("fork");
                    exit(1);
                } else if (pid == 0) { //child
                    execvp(argv[0], argv);
                    exit(0);
                }
                else { //parent
                    int status;
                    wait(&status);
                }
            }
*/
        }

        for (i = 0; i < pipe_count + 1 && pipe_count != 0; i++) { //exist pipe
            if (i != pipe_count) pipe(pfd[i]);

            if (left1[l1] != -1) {
                if (right1[r1] != -1) { //< >
                    if (left1[l1] > right1[r1]) { //> <
                        //my_redirect(argc, argv, 1, right1, right_count1);
                    } else { //< >

                    }
                } else if (right2[r2] != -1) { //< >>

                } else { //<
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, i, 3, left1, l1++);
                }
            } else if (left2[l2] != -1) {
                if (right1[r1] != -1) { //<< >

                } else if (right2[r2] != -1) { // << >>

                } else { //<<
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, i, 4, left2, l2++);
                }
            } else {
                if (right1[r1] != -1) { //>
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, i, 1, right1, r1++);
                } else if (right2[r2] != -1) { // >>
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, i,  2, right2, r2++);
                } else { //no redirect
                    my_exec(argc, argv, &fd, pipe_locate[0], pfd, i,  0, right1, 0);
                    //printf("AAA\n");
                }
            }

/*
            struct command_table *p;
            for (p = b_cmd_tbl; p->cmd; p++) {
                if (strcmp(argv[pipe_locate[i] + 1], p->cmd) == 0) { //if not built-in command
                    (*p->func)(argc, argv + pipe_locate[i] + 1);
                    break;
                }
            }
            if (p->cmd == NULL) { //if built-in command
                if ((pid = fork()) < 0) { //error
                    perror("fork");
                    exit(1);
                } else if (pid == 0) { //child
                    my_dup(i, pipe_count, pfd);
                    execvp(argv[pipe_locate[i] + 1], argv + pipe_locate[i] + 1);
                    exit(0);
                } else { //parent
                    if (i > 0) {
                       close(pfd[i - 1][0]); close(pfd[i - 1][1]);
                   }
                }
            }
*/
        }
        int status;
/*
        for (i = 0; i < pipe_count + 1; i++) {
            printf("BBB\n");
            wait(&status);
        }
*/
    }
    return 0;
}
