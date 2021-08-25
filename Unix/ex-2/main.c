// 61820353 Kohki Yamada
#include <stdio.h>
#include <string.h>
#include "buf.h"

int main()
{
    int i, argc;
    char *argv[BUFLENGTH], lbuf[NARGS];
    extern struct command_table cmd_tbl[];
    struct command_table *p;

    init();

    while (1) {
        printf("$ ");

        if (fgets(lbuf, sizeof(lbuf), stdin) == NULL) {
            putchar('\n');
            return 0;
        }
        lbuf[strlen(lbuf) - 1] = '\0';
        if (*lbuf == '\0')
            continue;

        getargs(&argc, argv, lbuf);

        for (p = cmd_tbl; p->cmd; p++) {
            if (strcmp(argv[0], p->cmd) == 0) {
                (*p->func)(argc, argv);
                break;
            }
        }
        if (p->cmd == NULL)
            fprintf(stderr, "Unknown Command\n");
    }

    return 0;
}
