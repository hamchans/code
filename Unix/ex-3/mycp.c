#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

void mycp(char *file1, char *file2);

int main()
{
    char file1[16], file2[16];
    scanf("%s %s", file1, file2);
    mycp(file1, file2);
    return 0;
}

void mycp(char *file1, char *file2)
{
    char ans[4];
    if (fopen(file2, "r") == NULL) {
        if (fopen(file2, "w+") == NULL) {
            fprintf(stderr, "%d\n", errno);
            perror("open");
            return;
        }
        while(fgets(str, N, fp) != NULL) {
            printf("%s", str);
        }

    } else {
        fprintf(stdout, "%s already exists. Do you overwrite? [Yes/No]: ", file2);
        scanf("%s", ans);
        *ans = upperCase(ans);
        if (strcmp(ans, "YES") == 0) {
            fprintf(stdout, "Yes\n");
        } else if (strcmp(ans, "Y") == 0) {
            fprintf(stdout, "Y\n");
        } else if (strcmp(ans, "No") == 0) {
            fprintf(stdout, "No\n");
        } else if (strcmp(ans, "N") == 0) {
            fprintf(stdout, "N\n");
        } else {
            fprintf(stderr, "Input [Yes/No] or [Y/N]\n");
        }
        fprintf(stdout, "%s\n", ans);
    }
}
