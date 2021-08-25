#include <stdio.h>

#define BUFLENGTH   256
#define NARGS       256


int main()
{
    int argc;
    char *argv[BUFLENGTH], lbuf[NARGS];
    while (1) {
        printf("mysh$ ");

        if (fgets(lbuf, sizeof(lbuf), stdin) == NULL) {
            putchar('\n');
            return 0;
        }

        lbuf[strlen(lbuf) - 1] = '\0';
        if (*lbuf == '\0')
            continue;

        gettoken();
    }
    return 0;
}
