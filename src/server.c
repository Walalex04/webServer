
#include "server.h"

#include <stdio.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>



/**
 * @brief Init the server 
 * 
 * this function enable de server, use the port 8080 by defect
 * 
 * @param config the pointer of the server's configuration
 */
void initilizeServer(server *config){

    //we get the information about the server

    struct addrinfo *myserver = config->info;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;

    
    int sockfd;

    sockfd =  socket(myserver->ai_family, myserver->ai_socktype, myserver->ai_protocol);

    //verify the status
    if (sockfd == -1)
    {
        perror("don't create the socket. Sever Failed!");
        return 1;
    }

    //conect with the port
    if(bind(sockfd, myserver->ai_addr, myserver->ai_addrlen) == -1){
        perror("don't make the bind, Server Failed!");
        return 2;
    }

    //put to listen the server
    if(listen(sockfd, config->backlog) == -1){
        perror("cannot put to listen the server. Server Failed!");
        return 3;
    }

    //accept the comunicacion, this part we must do it using process to 
    //manage as better as posible the different connections

    //must be enter in a while
    addr_size = sizeof their_addr;
    int new_fd = accept(sockfd, (struct sockaddr *)&their_addr)


    close(new_fd);
    close(sockfd);

    return 0;

}