#define BUFLENGTH   256
#define NARGS       256

#define PATHNAME_SIZE 512
#define ROOT_DIRECTORY "/Users/kohki.y"

#define TKN_NORMAL 1
#define TKN_REDIR_IN 2
#define TKN_REDIR_OUT 3
#define TKN_PIPE 4
#define TKN_BG 5
#define TKN_EOL 6
#define TKN_EOF 7


char pathname[PATHNAME_SIZE];

struct command_table {
    char *cmd;
    void (*func)(int, char *[]);
};

void gettoken(int *argc, char *argv[], char *lbuf);
void setargs(int *argc, char *argv[], char *lbuf);

void my_cd(int argc, char *argv[]);
void my_pwd(int argc, char *argv[]);
//void my_ls(int argc, char *argv[]);
void my_exit(int argc, char *argv[]);
void exec_b_cmd(int argc, char *argv[]);
void my_dup(int i, int pipe_count, int pfd[9][2]);
void my_exec(int argc, char *argv[], int *fd, int pipe_locate_num, int pfd[9][2], int i, int redirect, int redirect_location[10], int rd_num);
void my_redirect(int argc, char *argv[], int *fd, int redirect, int redirect_location[10], int rd_num);
void ignore();
//void cleanupRoutine(int signal_number);
