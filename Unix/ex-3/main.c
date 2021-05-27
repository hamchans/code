//61820353 Kohki Yamada

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "mysh.h"

int main()
{
    int argc;
    char *argv[BUFLENGTH], lbuf[NARGS];
    extern char pathname[PATHNAME_SIZE];
    memset(pathname, '\0', PATHNAME_SIZE);

    while (1) {
        setargs(&argc, argv, lbuf);
        printf("mysh$ ");
        gettoken(&argc, argv, lbuf);
        exec_cmd(argc, argv);
    }
    return 0;
}
