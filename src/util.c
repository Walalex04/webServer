
#include <stdio.h>
#include <netinet/in.h>  // Primero headers del sistema
#include <arpa/inet.h>
#include "util.h"
#include "string.h"



/**
 * @brief Print IP using a version 4
 *
 * Make a format the ip that is send via the pointer for next printing via console 
 *
 * @param socAddr It is the struct addr's pointer
 */
void printIPv4(struct sockaddr_in *socAddr){
  
  char ip4[INET_ADDRSTRLEN]; 
  
  inet_ntop(AF_INET, &(socAddr->sin_addr), ip4, INET_ADDRSTRLEN);
  
  printf("The IPv4 address is: %s\n", ip4);
}


/**
 * @brief Print IP using a version 6
 *
 * Make a format the ip that is send via the pointer for next printing via console 
 *
 * @param socAddr It is the struct addr's pointer
 */

void printIPv6(struct sockaddr_in6 *socAddr6){
  
  char ip6[INET6_ADDRSTRLEN];

  inet_ntop(AF_INET6, &(socAddr6->sin6_addr), ip6, INET_ADDRSTRLEN);

  printf("The IPv6 address is %s\n", ip6);
}





