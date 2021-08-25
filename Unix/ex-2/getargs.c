#include <string.h>
#include <ctype.h>
#include "buf.h"

void getargs(int *argc, char *argv[], char *lbuf)
{
    *argc = 0;
    argv[0] = NULL;

    while (1) {
        while (isblank(*lbuf))
            lbuf++;
        if (*lbuf == '\0')
            return;
        argv[(*argc)++] = lbuf;
        while (*lbuf && !isblank(*lbuf))
            lbuf++;
        if (*lbuf == '\0')
            return;
        *lbuf++ = '\0';
    }
}
