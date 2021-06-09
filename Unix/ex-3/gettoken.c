#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mysh.h"


void gettoken(int *argc, char *argv[], char *lbuf)
{
    int pc = 1;
    char ch;
    char *token = lbuf;
    while ((ch = getc(stdin)) != '\n') {
        switch (ch) {
            case '-':
                if (*(token - 1) != ' ') {
                    *token++ = ch;
                    break;
                }
                *token++ = ' ';
                *token++ = ch;
                break;
            case ' ':
                *token++ = ch;
                break;
            case '|':
            case '>':
            case '<':
            case '&':
            case ';':
                *token++ = ' ';
                *token++ = ch;
                //while ((ch = getc(stdin)) == ' ');
                ch = getc(stdin);
                if (ch == '\n')
                    break;
                switch (ch) {
                    case '|':
                    case '>':
                    case '<':
                    case '&':
                        *token++ = ch;
                        *token++ = ' ';
                        break;
                    default:
                        *token++ = ' ';
                        *token++ = ch;
                        break;
                }
                break;
            default:
                *token++ = ch;
                break;
        }
        if (ch == '\n')
            break;
    }

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

void setargs(int *argc, char *argv[], char *lbuf)
{
    int i;
    for (i=0; i<*argc; i++)
        argv[i] = NULL;
    *argc = 0;

    for (i=0; i<NARGS; i++)
        lbuf[i] = ' ';
    lbuf[strlen(lbuf) - 1] = '\0';
}
