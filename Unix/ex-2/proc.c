#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "buf.h"

extern struct buf_header hash_head[HASH];
extern struct buf_header freelist;

struct command_table cmd_tbl[] = {
    {"help", help_proc},
    {"init", init_proc},
    {"buf", buf_proc},
    {"hash", hash_proc},
    {"free", free_proc},
    {"getblk", getblk_proc},
    {"brelse", brelse_proc},
    {"set", set_proc},
    {"reset", reset_proc},
    {"quit", quit_proc},
    {"NULL", NULL}
};

void init()
{
    int i;
    for (i=0; i<HASH; i++) {
        hash_head[i].hash_fp = &hash_head[i];
        hash_head[i].hash_bp = &hash_head[i];
    }
    freelist.free_fp = &freelist;
    freelist.free_bp = &freelist;
}

void help_proc(int argc, char *argv[])
{
    printf("help:\t\t\t show help\n");
    printf("init:\t\t\t initialize hash list and free list\n");
    printf("buf [n]:\t\t show buffer headers\n");
    printf("hash [n]:\t\t show hash lists\n");
    printf("free:\t\t\t show free list\n");
    printf("getblk n:\t\t get block 'n'\n");
    printf("brelse n:\t\t release block 'n'\n");
    printf("set n stat [stat ...]:\t set status 'stat' in block 'n'\n");
    printf("reset n stat [stat ...]: reset status 'stat in block n'\n");
    printf("quit:\t\t\t quit\n");
}

void init_proc(int argc, char *argv[])
{
    int i;
    int initial[INIT] = {28, 4, 64, 17, 5, 97, 98, 50, 10, 3, 35, 99};
    int initial_free[FREE] = {3, 5, 4, 28, 97, 10};

    if (argc == 1) {
        struct buf_header *p;

        if ((p = (struct buf_header *) malloc(sizeof(struct buf_header) * INIT)) == NULL) {
            fprintf(stderr, "Can't allocate memory!");
            exit(1);
        }

        for (i=0; i<INIT; i++) {
            printf("scenario2\n");
            int hash = hash_cal(initial[i]);
            p->blkno = initial[i];
            p->stat = (STAT_VALID | STAT_LOCKED);
            insert_hash(&hash_head[hash], p++, -1);
        }

        for (i=0; i<FREE; i++) {
            p = search_hash(initial_free[i]);
            p->stat &= ~STAT_LOCKED;
            printf("buf(blkno %d) was inserted at tail of free list\n", p->blkno);
            insert_free(&freelist, p++, -1);
        }
    } else {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    }
}

void buf_proc(int argc, char *argv[])
{
    int i, j, count = 0, flag = 0, find = 0;
    struct buf_header *p;

    if (argc == 1) {
        for (i=0; i<HASH; i++) {
            for (p=hash_head[i].hash_fp; p!=&hash_head[i]; p=p->hash_fp) {
                char STAT[6] = "------";
                stat_calc(p, STAT);
                printf("[%2d: %2d %c%c%c%c%c%c]\n", count++, p->blkno, STAT[0], STAT[1], STAT[2], STAT[3], STAT[4], STAT[5]);
            }
        }
    } else if (argc >= 2) {
        for (i=1; i<argc; i++) {
            flag = 0;
            count = 0;
            for (j=0; argv[i][j] != '\0'; j++) {
                if (argv[i][j] < '0' || argv[i][j] > '9') {
                    fprintf(stderr, "Please type the correct value!\n");
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
                continue;
            int n = strtol(argv[i], 0, 10);
            for (j=0; j<HASH; j++) {
                find = 0;
                for (p=hash_head[j].hash_fp; p!=&hash_head[j]; p=p->hash_fp) {
                    if (n == count) {
                        char STAT[6] = "------";
                        stat_calc(p, STAT);
                        printf("[%2d: %2d %c%c%c%c%c%c]\n", count++, p->blkno, STAT[0], STAT[1], STAT[2], STAT[3], STAT[4], STAT[5]);
                        find = 1;
                        break;
                    }
                    else
                        count++;
                }
                if (find == 1)
                    break;
            }
            if (find == 0)
                fprintf(stderr, "Not exisit No.%d buffer!\n", n);
        }
    } else {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    }
}

void hash_proc(int argc, char *argv[])
{
    int i, j, count = 0, flag = 0, find = 0;
    struct buf_header *p;

    if (argc == 1) {
        for (i=0; i<HASH; i++) {
            for (p=hash_head[i].hash_fp; p!=&hash_head[i]; p=p->hash_fp) {
                char STAT[6] = "------";
                stat_calc(p, STAT);
                printf("%d: [%2d: %2d %c%c%c%c%c%c] ", i, count++, p->blkno, STAT[0], STAT[1], STAT[2], STAT[3], STAT[4], STAT[5]);
            }
            if (p->hash_bp != &hash_head[i])
                printf("\n");
        }
    } else if (argc >= 2) {
        for (i=1; i<argc; i++) {
            flag = 0;
            count = 0;
            for (j=0; argv[i][j] != '\0'; j++) {
                if (argv[i][j] < '0' || argv[i][j] > '9') {
                    fprintf(stderr, "Please type the correct value!\n");
                    flag = 1;
                    break;
                }
            }
            if (flag == 1)
                continue;
            int n = strtol(argv[i], 0, 10);
            for (j=0; j<HASH; j++) {
                find = 0;
                for (p=hash_head[j].hash_fp; p!=&hash_head[j]; p=p->hash_fp) {
                    if (j == n) {
                        char STAT[6] = "------";
                        stat_calc(p, STAT);
                        printf("%d: [%2d: %2d %c%c%c%c%c%c] ", j, count++, p->blkno, STAT[0], STAT[1], STAT[2], STAT[3], STAT[4], STAT[5]);
                        find = 1;
                    } else
                        count++;
                }
                if (p == &hash_head[n] && p->hash_bp != &hash_head[n])
                    printf("\n");
                if (find == 1)
                    break;
            }
            if (find == 0)
                fprintf(stderr, "Not exisit No.%d buffer!\n", n);
        }
    } else {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    }
}

void free_proc(int argc, char *argv[])
{
    int i, count = 0, flag = 1;
    struct buf_header *p, *q;

    if (argc == 1) {
        for (p=freelist.free_fp; p!=&freelist; p=p->free_fp) {
            flag = 1;
            count = 0;
            while (1) {
                if (flag != 1)
                    break;
                for (i=0; i<HASH; i++) {
                    if (flag != 1)
                        break;
                    for (q = hash_head[i].hash_fp; q!=&hash_head[i]; q=q->hash_fp) {
                        if (flag != 1)
                            break;
                        if (q->blkno != p->blkno)
                            count++;
                        else
                            flag = 0;
                    }
                }
            }
            char STAT[6] = "------";
            stat_calc(p, STAT);
            printf("[%2d: %2d %c%c%c%c%c%c] ", count, p->blkno, STAT[0], STAT[1], STAT[2], STAT[3], STAT[4], STAT[5]);
        }
        if (p == &freelist && p->free_bp != &freelist)
            printf("\n");
    } else {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    }
}

void getblk_proc(int argc, char *argv[])
{
    int i;
    if (argc == 1) {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    } else if (argc == 2) {
        for (i=0; argv[1][i] != '\0'; i++) {
            if (argv[1][i] < '0' || argv[1][i] > '9') {
                fprintf(stderr, "Please type the correct value!\n");
                return;
            }
        }
        int n = strtol(argv[1], 0, 10);
        if (search_hash(n) != NULL)
            getblk(n);
        else {
            fprintf(stderr, "Not exisit No.%d buffer!\n", n);
            return;
        }
    } else {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    }
}

void brelse_proc(int argc, char *argv[])
{
    int i;
    struct buf_header *p;
    if (argc == 1) {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    } else if (argc == 2) {
        for (i=0; argv[1][i] != '\0'; i++) {
            if (argv[1][i] < '0' || argv[1][i] > '9') {
                fprintf(stderr, "Please type the correct value!\n");
                return;
            }
        }
        int n = strtol(argv[1], 0, 10);
        if ((p = search_hash(n)) != NULL)
            brelse(p);
        else {
            fprintf(stderr, "Not exisit No.%d buffer!\n", n);
            return;
        }
    } else {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    }
}

void set_proc(int argc, char *argv[])
{
    int i;
    struct buf_header *p;
    if (argc == 1) {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    } else if (argc == 2) {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    } else if (argc == 3) {
        for (i=0; argv[1][i] != '\0'; i++) {
            if (argv[1][i] < '0' || argv[1][i] > '9') {
                fprintf(stderr, "Please type the correct value!\n");
                return;
            }
        }
        int n = strtol(argv[1], 0, 10);
        if ((p = search_hash(n)) != NULL) {
            for (i=0; argv[2][i] != '\0'; i++) {
                if (argv[2][i] == 'L') {
                    if ((p->stat & STAT_LOCKED) != STAT_LOCKED) {
                        p->stat |= STAT_LOCKED;
                        remove_from_free(p);
                    }
                }
                else if (argv[2][i] == 'V')
                    p->stat |= STAT_VALID;
                else if (argv[2][i] == 'D')
                    p->stat |= STAT_DWR;
                else if (argv[2][i] == 'K')
                    p->stat |= STAT_KRDWR;
                else if (argv[2][i] == 'W')
                    p->stat |= STAT_WAITED;
                else if (argv[2][i] == 'O')
                    p->stat |= STAT_OLD;
                else
                    fprintf(stderr, "%c is not correct stat!\n", argv[2][i]);
            }
        } else {
            fprintf(stderr, "Not exisit No.%d buffer!\n", n);
            return;
        }
    } else {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    }
}

void reset_proc(int argc, char *argv[])
{
    int i;
    struct buf_header *p;
    if (argc == 1) {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    } else if (argc == 2) {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    } else if (argc == 3) {
        for (i=0; argv[1][i] != '\0'; i++) {
            if (argv[1][i] < '0' || argv[1][i] > '9') {
                fprintf(stderr, "Please type the correct value!\n");
                return;
            }
        }
        int n = strtol(argv[1], 0, 10);
        if ((p = search_hash(n)) != NULL) {
            for (i=0; argv[2][i] != '\0'; i++) {
                if (argv[2][i] == 'L') {
                    if ((p->stat & STAT_LOCKED) == STAT_LOCKED) {
                        if ((p->stat & STAT_OLD) == STAT_OLD) {
                            p->stat &= ~(STAT_OLD | STAT_LOCKED);
                            insert_free(&freelist, p, 0);
                            printf("buf(blkno %d) was inserted at beginning of free list\n", p->blkno);
                        } else {
                            p->stat &= ~STAT_LOCKED;
                            insert_free(&freelist, p, -1);
                            printf("buf(blkno %d) was inserted at tail of free list\n", p->blkno);
                        }
                    }
                }
                else if (argv[2][i] == 'V')
                    p->stat &= ~STAT_VALID;
                else if (argv[2][i] == 'D')
                    p->stat &= ~STAT_DWR;
                else if (argv[2][i] == 'K')
                    p->stat &= ~STAT_KRDWR;
                else if (argv[2][i] == 'W')
                    p->stat &= ~STAT_WAITED;
                else if (argv[2][i] == 'O') {
                    p->stat &= ~STAT_OLD;
                }
                else {
                    fprintf(stderr, "%c is not correct stat!\n", argv[2][i]);
                    return;
                }
            }
        } else {
            fprintf(stderr, "Not exisit No.%d buffer!\n", n);
            return;
        }
    } else {
        fprintf(stderr, "Please type in the correct syntax!\n");
        return;
    }
}

void quit_proc(int argc, char *argv[])
{
    exit(0);
}

void stat_calc(struct buf_header *p, char *stat)
{
    int i, j;
    char A[6] = "OWKDVL";
    int reg = p->stat;

    for (i=0; i<6; i++) {
        if (reg %2 == 1)
            stat[5-i] = A[5-i];
        reg /= 2;
    }

}
