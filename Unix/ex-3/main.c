//61820353 Kohki Yamada

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mysh.h"



int main()
{
    int argc;
    char *argv[BUFLENGTH], lbuf[NARGS];
    //char *token;
    //extern struct command_table cmd_tbl[];
    extern char pathname[PATHNAME_SIZE];
    //struct command_table *p;

    memset(pathname, '\0', PATHNAME_SIZE);

    while (1) {
/*
        for (int i=0; i<argc; i++)
            argv[i] = NULL;
        argc = 0;

        for (int i=0; i<NARGS; i++)
            lbuf[i] = ' ';
        lbuf[strlen(lbuf) - 1] = '\0';
*/
        setargs(&argc, argv, lbuf);
        printf("mysh$ ");

        gettoken(&argc, argv, lbuf);
/*
        for (int i=0; i<argc; i++)
            printf("%s\n", argv[i]);
*/
        exec_cmd(argc, argv);
/*
        for (p = cmd_tbl; p->cmd; p++) {
            if (strcmp(argv[0], p->cmd) == 0) {
                (*p->func)(argc, argv);
                break;
            }
        }
        if (p->cmd == NULL)
            fprintf(stderr, "mysh: command not found: %s\n", argv[0]);
*/
    }
    return 0;
}
