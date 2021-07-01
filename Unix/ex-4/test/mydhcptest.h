//status
#define INIT                1
#define FIN                 0

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
#define HALF_LIMIT          116
#define LIMIT               117

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



//client side
void mydhcp_discover();
void mydhcp_wait_offer();
void mydhcp_request();
void mydhcp_wait_ack();
/*
void mydhcp_in_use();
void mydhcp_select();
void mydhcp_release_client();
*/
int wait_client_event();


//server side

void mydhcp_wait_discover();
void mydhcp_offer();
void mydhcp_wait_request();
void mydhcp_ack();
/*
void mydhcp_release_server();
*/
int wait_server_event();


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
