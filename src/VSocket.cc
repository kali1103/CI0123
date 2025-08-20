/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2025-ii
  *  Grupos: 6
  *
  ****** VSocket base class implementation
  *
  * (Fedora version)
  *
 **/

#include <sys/socket.h>
#include <arpa/inet.h>		// ntohs, htons
#include <stdexcept>            // runtime_error
#include <cstring>		// memset
#include <netdb.h>			// getaddrinfo, freeaddrinfo
#include <unistd.h>			// close
/*
#include <cstddef>
#include <cstdio>

//#include <sys/types.h>
*/
#include "VSocket.h"


/**
  *  Class creator (constructor)
  *     use Unix socket system call
  *
  *  @param     char t: socket type to define
  *     's' for stream
  *     'd' for datagram
  *  @param     bool ipv6: if we need a IPv6 socket
  *
 **/
void VSocket::BuildSocket( char t, bool IPv6 ){

   int domain = AF_INET;
   int type = SOCK_STREAM;

   if(t == 'd') {
      type = SOCK_DGRAM;
   }
   if(IPv6 == true) {
      domain = AF_INET6;
   }
   this->idSocket = socket(domain, type, 0);

   if ( -1 == this->idSocket ) {
      throw std::runtime_error( "VSocket::BuildSocket, (reason)" );
   }
   
}


/**
  * Class destructor
  *
 **/
VSocket::~VSocket() {

   this->Close();

}


/**
  * Close method
  *    use Unix close system call (once opened a socket is managed like a file in Unix)
  *
 **/
void VSocket::Close(){
   if ( -1 != this->idSocket) {
      if (close(this->idSocket) == -1) {
         throw std::runtime_error("VSocket::Close failed: " + std::string(strerror(errno)));
      }
      this->idSocket = -1;
   }
   else {
        throw std::runtime_error("VSocket::Close, socket already closed.");
   }
}


/**
  * EstablishConnection method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dot notation, example "10.84.166.62"
  * @param      int port: process address, example 80
  *
 **/
int VSocket::EstablishConnection( const char * hostip, int port ) {

   struct sockaddr_in host4;
   memset(&host4, 0, sizeof(host4));

   host4.sin_family = AF_INET;
   if (inet_pton(AF_INET, hostip, &host4.sin_addr) <= 0) {
      throw std::runtime_error("IP invalida");
   }
   host4.sin_port = htons(port);

   if ( -1 == connect(this->idSocket, (sockaddr*)&host4, sizeof(host4))) {
      throw std::runtime_error("Connect failed: " + std::string(strerror(errno)));
   }

   return 0;

}


/**
  * EstablishConnection method
  *   use "connect" Unix system call
  *
  * @param      char * host: host address in dns notation, example "os.ecci.ucr.ac.cr"
  * @param      char * service: process address, example "http"
  *
 **/
int VSocket::EstablishConnection( const char *host, const char *service ) {
   struct addrinfo hints{}, *res, *rp;
   hints.ai_family = AF_UNSPEC;     // IPv4 o IPv6
   hints.ai_socktype = this->type; // TCP o UDP

   int st = getaddrinfo(host, service, &hints, &res);
   if (st != 0) {
      throw std::runtime_error("getaddrinfo failed: " + std::string(gai_strerror(st)));
   }

   int success = -1;
   for (rp = res; rp != nullptr; rp = rp->ai_next) {
      if (connect(idSocket, rp->ai_addr, rp->ai_addrlen) == 0) {
         success = 0;
         break;
      }
   }

   freeaddrinfo(res);

   if (success != 0) {
      throw std::runtime_error("Unable to connect to host");
   }

   return 0;
}

