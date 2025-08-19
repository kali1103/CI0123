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
   this->type = SOCK_STREAM;
   this->IPv6 = AF_INET;

   if(t == 'd') {
      this->type = SOCK_DGRAM;
   }

   if(this->IPv6 == true) {
      this->IPv6 = AF_INET6;
   }
   this->idSocket = socket(domain, this->type, this->IPv6);
   
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

   int st = -1;

   if ( -1 == st ) {
      throw std::runtime_error( "VSocket::EstablishConnection" );
   }

   return st;

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
   int st = -1;

   throw std::runtime_error( "VSocket::EstablishConnection" );

   return st;

}

