#define HASH 4
#define INIT 12
#define FREE 6
#define BUFLENGTH 256
#define NARGS 256

#define STAT_LOCKED 0x00000001
#define STAT_VALID  0x00000002
#define STAT_DWR    0x00000004
#define STAT_KRDWR  0x00000008
#define STAT_WAITED 0x00000010
#define STAT_OLD    0x00000020

struct buf_header {
    int blkno;
    struct buf_header *hash_fp;
    struct buf_header *hash_bp;
    unsigned int stat;
    struct buf_header *free_fp;
    struct buf_header *free_bp;
    char *cashe_data;
};

struct buf_header hash_head[HASH];
struct buf_header freelist;

int hash_cal(int blkno);
void insert_head(struct buf_header *h, struct buf_header *p);
void insert_tail(struct buf_header *h, struct buf_header *p);
void insert_hash(struct buf_header *h, struct buf_header *p, int where);
struct buf_header *search_hash(int blkno);
void insert_head_free(struct buf_header *h, struct buf_header *p);
void insert_tail_free(struct buf_header *h, struct buf_header *p);
void insert_free(struct buf_header *h, struct buf_header *p, int where);
void remove_from_hash(struct buf_header *p);
void remove_from_free(struct buf_header *p);

struct buf_header *getblk(int blkno);
void brelse(struct buf_header *buffer);

void getargs(int *argc, char *argv[], char *lbuf);

void init();
void help_proc(int argc, char *argv[]);
void init_proc(int argc, char *argv[]);
void buf_proc(int argc, char *argv[]);
void hash_proc(int argc, char *argv[]);
void free_proc(int argc, char *argv[]);
void getblk_proc(int argc, char *argv[]);
void brelse_proc(int argc, char *argv[]);
void set_proc(int argc, char *argv[]);
void reset_proc(int argc, char *argv[]);
void quit_proc(int argc, char *argv[]);
void stat_calc(struct buf_header *p, char *stat);

struct command_table {
    char *cmd;
    void (*func)(int, char *[]);
};
