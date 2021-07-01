#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mydhcp.h"

int server_status = INIT;

int main(int argc, char *argv[])
{
    FILE *fp;
    char filename[12];
    char ch;

    if (argc == 1) {
        fprintf(stderr, "Please input a config-file.\n");
        exit(1);
    } else if (argc == 2) {
        memcpy(filename, argv[1], 12);
        if((fp = fopen(filename, "r")) == NULL) {
            fprintf(stderr, "%s file cannot open!\n", filename);
            exit(1);
    	}
    } else {
        fprintf(stderr, "Please input a correct syntax.\n");
        exit(1);
    }

    while((ch = fgetc(fp)) != EOF) {
        putchar(ch);
    }


    









    mydhcp_wait_discover();
    //←DHCPDISCOVER
    //→DHCPOFFER
    mydhcp_ack();
    //←DHCPREQUEST
    //→DHCPACK

    //←DHCPREQUEST
    //→DHCPACK
    mydhcp_release_server();
    //←DHCPRELEASE
    fclose(fp);

    return 0;
}
