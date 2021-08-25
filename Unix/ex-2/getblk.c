#include <stdio.h>
#include "buf.h"

extern struct buf_header hash_head[HASH];
extern struct buf_header freelist;

int hash_cal(int blkno)
{
    return blkno % HASH;
}

void insert_head(struct buf_header *h, struct buf_header *p)
{
    p->hash_bp = h;
    p->hash_fp = h->hash_fp;
    h->hash_fp->hash_bp = p;
    h->hash_fp = p;
}

void insert_tail(struct buf_header *h, struct buf_header *p)
{
    p->hash_fp = h;
    p->hash_bp = h->hash_bp;
    h->hash_bp->hash_fp = p;
    h->hash_bp = p;
}

void insert_hash(struct buf_header *h, struct buf_header *p, int where)
{
    if (where == 0) // 0 is head and -1 is tail. Others are NULL.
        insert_head(h, p);
    else if (where == -1)
        insert_tail(h, p);
}

struct buf_header *search_hash(int blkno)
{
    struct buf_header *p;

    int hash = hash_cal(blkno);
    for (p = hash_head[hash].hash_fp; p != &hash_head[hash]; p = p->hash_fp) {
        if (p->blkno == blkno)
            return p;
    }
    return NULL;
}

void insert_head_free(struct buf_header *h, struct buf_header *p)
{
    p->free_bp = h;
    p->free_fp = h->free_fp;
    h->free_fp->free_bp = p;
    h->free_fp = p;
}

void insert_tail_free(struct buf_header *h, struct buf_header *p)
{
    p->free_fp = h;
    p->free_bp = h->free_bp;
    h->free_bp->free_fp = p;
    h->free_bp = p;
}

void insert_free(struct buf_header *h, struct buf_header *p, int where)
{
    if (where == 0) // 0 is head and -1 is tail. Others are NULL.
        insert_head_free(h, p);
    else if (where == -1)
        insert_tail_free(h, p);
}

void remove_from_hash(struct buf_header *p)
{
    p->hash_bp->hash_fp = p->hash_fp;
    p->hash_fp->hash_bp = p->hash_bp;
    p->hash_fp = p->hash_bp = NULL;
}

void remove_from_free(struct buf_header *p)
{
    p->free_bp->free_fp = p->free_fp;
    p->free_fp->free_bp = p->free_bp;
    p->free_fp = p->free_bp = NULL;
}

struct buf_header *getblk(int blkno)
{
    struct buf_header *p;
    while (1 /*buffer not found*/) {
        if (search_hash(blkno) != NULL /*blkno in hash queue*/) {
            p = search_hash(blkno);
            if ((p->stat & STAT_LOCKED) == STAT_LOCKED /*buffer locked*/) {
                /*scenario5*/
                printf("scenario5\n");
                p->stat |= STAT_WAITED;
                //sleep(); /*event buffer becomes free*/
                printf("Process goes to sleep\n");
                return NULL;
                //continue;
            }

            /*scenario1*/
            printf("scenario1\n");
            p->stat |= STAT_LOCKED; //make buffer locked
            remove_from_free(p); //remove buffer from free list
            return p; //return pointer to buffer

        } else {
            if (freelist.free_fp == &freelist /*not buffer on free list*/) {
                /* scenario4*/
                printf("scenario4\n");
                // sleep(); /*event any buffer becomes free*/
                printf("Process goes to sleep\n");
                return NULL;
                //continue;
            }
            p = freelist.free_fp;
            p->stat |= STAT_LOCKED; //make buffer locked
            remove_from_free(p); /*remove buffer from free list;*/
            if ((p->stat & STAT_DWR) == STAT_DWR /*buffer marked for delayed write*/) {
                /*scenario3*/
                printf("scenario3\n");
                p->stat |= (STAT_KRDWR | STAT_OLD); //write to HDD

                /*asynchronous write buffer to disk;*/
                continue;
            }
            /*scenario2*/
            printf("scenario2\n");
            remove_from_hash(p); //remove buffer from old hash queue

            p->blkno = blkno; //set new blkno
            p->stat &= ~STAT_VALID; //not valid
            p->stat |= STAT_KRDWR; //read from HDD
            p->stat &= ~STAT_KRDWR; //finish reading from HDD
            p->stat |= STAT_VALID; //finish reading from HDD

            insert_hash(&hash_head[blkno % HASH], p, -1); //put buffer onto new hash queue
            return p; //return pointer to buffer
        } /*end of "else"*/
    } /*end of while()*/
}

void brelse(struct buf_header *buffer)
{
    if (freelist.free_fp == &freelist) {
        // wakeup(); //wakeup all procs; event, waiting for any buffer to become free; //scenario4
        printf("Wakeup processes waiting for any buffer\n");
    }
    if ((buffer->stat & STAT_LOCKED) == STAT_LOCKED) {
        // wakeup(); //wakeup all procs; event, waiting for this buffer to become free; //scenario5
        printf("Wakeup processes waiting for buffer of blkno %d\n", buffer->blkno);
        buffer->stat &= ~STAT_WAITED;
    }

    /*critical section begin*/
    // raise_cpu_level(); //raise processor execution level to block interrupts;
    if (((buffer->stat & STAT_VALID) == STAT_VALID) && ((buffer->stat & STAT_OLD) != STAT_OLD) /*buffer contents valid and buffer not old*/) {
        insert_free(&freelist, buffer, -1); //enqueue buffer at end of free list;
        printf("buf(blkno %d) was inserted at tail of free list\n", buffer->blkno);
    } else {
        buffer->stat &= ~(STAT_OLD | STAT_KRDWR);
        insert_free(&freelist, buffer, 0); //enqueue buffer at beginning of free list; //scenario3
        printf("buf(blkno %d) was inserted at beginning of free list\n", buffer->blkno);
    }
    // lower_cpu_level(); //lower processor execution level to allow interrupts;
    /*critical section end*/

    buffer->stat &= ~STAT_LOCKED; //unlock(buffer);
}
