/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2025-i
  *  Grupos: 1 y 3
  *
  * (Fedora version)
  *
  *   Client side implementation of IPv6 UDP client-server model 
  *
 **/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h>

#include "Socket.h"

#define PORT    1234 
#define MAXLINE 1024 

int main() {
   VSocket * client;
   int sockfd; 
   int n, len; 
   char buffer[MAXLINE]; 
   char *hello = (char *) "Hello from client"; 
   struct sockaddr_in6 other;

   client = new Socket( 'd', true );

   memset( &other, 0, sizeof( other ) ); 
   
   other.sin6_family = AF_INET6; 
   other.sin6_port = htons(PORT); 
   n = inet_pton(AF_INET6, "::1", &other.sin6_addr);
   if (n != 1) {
      printf( "Error converting from IP address\n" );
      exit ( 23 );
   }

   n = client->sendTo( (void *) hello, strlen( hello ), (void *) & other ); 
   printf("Client: Hello message sent.\n"); 
   
   n = client->recvFrom( (void *) buffer, MAXLINE, (void *) & other );
   buffer[n] = '\0'; 
   printf("Client message received: %s\n", buffer); 

   client->Close(); 

   return 0;
 
} 

