#include <string.h>
#include <ctype.h>
#include "myftp.h"

char pathname[PATHNAME_SIZE];

void getargs(int *argsc, char *argsv[], char *lbuf)
{
    *argsc = 0;
    argsv[0] = NULL;

    while (1) {
        while (isblank(*lbuf))
            lbuf++;
        if (*lbuf == '\0')
            return;
        argsv[(*argsc)++] = lbuf;
        while (*lbuf && !isblank(*lbuf))
            lbuf++;
        if (*lbuf == '\0')
            return;
        *lbuf++ = '\0';
    }
}
