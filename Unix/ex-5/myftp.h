#define BUFLENGTH       256
#define NARGS           256
#define DATASIZE        128

#define PATHNAME_SIZE 512
#define ROOT_DIRECTORY "/home/md202/ub396535"

#define TYPE_QUIT       0x01
#define TYPE_PWD        0x02
#define TYPE_CWD        0x03
#define TYPE_LIST       0x04
#define TYPE_RETR       0x05
#define TYPE_STOR       0x06

#define TYPE_OK         0x10
#define TYPE_CMD_ERR    0x11
#define TYPE_FILE_ERR   0x12
#define TYPE_UNKWN_ERR  0x13

#define TYPE_DATA       0x20

#define CODE_0          0x00
#define CODE_1          0x01
#define CODE_2          0x02

struct command_table {
    char *cmd;
    void (*func)(int, char *[], int s);
};

void getargs(int *argsc, char *argsv[], char *lbuf);
void setargs(int *argc, char *argv[], char *lbuf);

void init_client();
void quit_client(int argsc, char *argsv[], int s);
void my_pwd(int argsc, char *argsv[], int s);
void my_cd(int argsc, char *argsv[], int s);
void my_dir(int argsc, char *argsv[], int s);
void my_lpwd(int argsc, char *argsv[], int s);
void my_lcd(int argsc, char *argsv[], int s);
void my_ldir(int argsc, char *argsv[], int s);
void my_get(int argsc, char *argsv[], int s);
void my_put(int argsc, char *argsv[], int s);
void my_help(int argsc, char *argsv[], int s);

struct myftp_message_format {
    unsigned char type;
    unsigned char code;
    unsigned short data_length;
    char message[DATASIZE];
};

struct myftp_format {
    unsigned int source_IP;
    unsigned int dest_IP;
    unsigned int source_port;
    unsigned int dest_port;
    struct myftp_message_format myftp_message;
};

struct myftp_format set_format(unsigned int source_IP, unsigned int dest_IP, unsigned int source_port, unsigned int dest_port, unsigned char type, unsigned char code, unsigned short datalen, char *message);

void my_send(int s, struct myftp_format format);
struct myftp_format my_recv(int s, struct myftp_format format);

void my_pwd_server(int s);
void my_cd_server(int s, struct myftp_format format);
void my_ls_server(int s, struct myftp_format format);
void my_get_server(int s, struct myftp_format f);
void my_put_server(int s, struct myftp_format f);
