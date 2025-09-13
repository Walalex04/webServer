
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "util.h"



int main(int argc, char *argv[]){

  int status;                               // store the status of getaddrinfo
  struct addrinfo *hints, *res, *p;

  memset(hints, 0, sizeof *(hints));        //make sure that is empty
  
  //config the hints
  hints->ai_family = AF_UNSPEC;             //don't care IPv4 or IPv6
  hints->ai_socktype = SOCK_STREAM;         //using TCP/IP
  hints->ai_flags = AI_PASSIVE;             //fill in my ip for me


  status = getaddrinfo(NULL, "8080", hints, res);
  //note that the NULL mean my ip


  if(status != 0){
    //there is an error
    fprintf(stderr, "gai_error: %s\n", gai_strerror(status));
    return EXIT_FAILURE;
  }



  //free linked-list
  freeaddrinfo(*res);

}
