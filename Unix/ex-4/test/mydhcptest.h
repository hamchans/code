//status
#define INIT                1
#define FIN                 2
#define ERROR               -1

//client side status
#define WAIT_OFFER          101
#define WAIT_ACK            102
#define HAVE_IP             103

//client side event
#define CONNECT_NETWORK     111
#define SEND_DISCOVER       112
#define RECEIVE_OFFER       113
#define SEND_REQUEST        114
#define RECEIVE_ACK         115
#define IN_USE              116
#define HALF_LIMIT          117
#define LIMIT               118
#define SEND_RELEASE        119
#define RECEIVE_OFFER_NG    120
#define RECEIVE_ACK_NG      121
#define RECEIVE_SIGALARM    122

//server side status
#define WAIT_DISCOVER       201
#define WAIT_REQUEST        202
#define ASSIGN_IP           203

//server side event
#define START               211
#define RECEIVE_DISCOVER    212
#define SEND_OFFER          213
#define RECEIVE_REQUEST     214
#define SEND_ACK            215
#define RECEIVE_RELEASE     216
#define RELEASE_IP          217

//limit time
//#define LIMIT_TIME          6





//client side
void mydhcp_discover();
void mydhcp_wait_offer();
void mydhcp_request();
void mydhcp_wait_ack();
void mydhcp_in_use_client();
void mydhcp_release_client();
void terminate();

void wait_client_event(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt, int st);
void set_client_status(int changed_status);


//server side

void mydhcp_wait_discover();
void mydhcp_offer();
void mydhcp_wait_request();
void mydhcp_ack();
void mydhcp_in_use_server();
void mydhcp_release_server();
void set_server_status(int changed_status);

void wait_server_event(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt);
void getargs(int *argsc, char *argsv[], char *lbuf);
int is_assign(int assign[], int argsc);

struct proctable {
    int status;
    int event;
    void (*func)(int s, struct sockaddr_in *skt, struct sockaddr_in *myskt);
};

struct mydhcp_message_format {
    unsigned char type;
    unsigned char code;
    unsigned short ttl;
    unsigned int IP;
    unsigned int Netmask;
};

struct mydhcp_format {
    unsigned int source_IP;
    unsigned int dest_IP;
    unsigned int source_port;
    unsigned int dest_port;
    struct mydhcp_message_format mydhcp_message;
};

struct mydhcp_format set_format(unsigned int source_IP, unsigned int dest_IP, unsigned int source_port, unsigned int dest_port, unsigned char type, unsigned char code, unsigned short ttl, unsigned int IP, unsigned int Netmask);
