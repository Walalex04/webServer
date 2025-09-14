


#ifndef UTIL_H

#define UTIL_H

#include <netinet/in.h> 

//print ip with format

void printIPv4(struct sockaddr_in *socAddr);
void printIPv6(struct sockaddr_in6 *socAddr6);



#endif
