

#ifndef SERVER_H
#define SERVER_H


#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


//define struct
typedef struct
{

    char port[5];
    int type;
    int protocol;
    int backlog;
    struct addrinfo *info;

} server;



//declare functions
void initilizeServer(server *config);


#endif