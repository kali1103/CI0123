/**
  *  Universidad de Costa Rica
  *  ECCI
  *  CI0123 Proyecto integrador de redes y sistemas operativos
  *  2025-i
  *  Grupos: 1 y 3
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

   int domain = AF_INET; // Socket IPv4.
   int type = SOCK_STREAM; // Socket tcp.
   if(t == 'd') {
      type = SOCK_DGRAM;
   }
   if(IPv6 == true) {
      domain = AF_INET6;
   }
   this->idSocket = socket(domain, type, 0);
   this->domain = domain; // Guardar el dominio para uso posterior
   this->type = type;
   if (this->idSocket < 0) {
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
   if (-1 != this->idSocket) {
      if(close(this->idSocket) == -1) {
         throw std::runtime_error("VSocket::Close failed: " + std::string(strerror(errno)));
      }
      this->idSocket = -1;
   }

   else {
      throw std::runtime_error( "VSocket::Close, socket already closed." );
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
int VSocket::EstablishConnection(const char *hostip, int port) {
    // Intenta IPv4
    struct sockaddr_in host4;
    memset(&host4, 0, sizeof(host4));
    host4.sin_family = AF_INET;
    if (inet_pton(AF_INET, hostip, &host4.sin_addr) == 1) {
        host4.sin_port = htons(port);
        if (-1 == connect(this->idSocket, (sockaddr*)&host4, sizeof(host4))) {
            throw std::runtime_error("Connect failed (IPv4): " + std::string(strerror(errno)));
        }
        return 0;
    }

    // Intenta IPv6
    struct sockaddr_in6 host6;
    memset(&host6, 0, sizeof(host6));
    host6.sin6_family = AF_INET6;
    if (inet_pton(AF_INET6, hostip, &host6.sin6_addr) == 1) {
        host6.sin6_port = htons(port);
        if (-1 == connect(this->idSocket, (sockaddr*)&host6, sizeof(host6))) {
            throw std::runtime_error("Connect failed (IPv6): " + std::string(strerror(errno)));
        }
        return 0;
    }

    // Si no es IP, asume DNS y usa getaddrinfo
    struct addrinfo hints{}, *res, *rp;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = this->domain; // AF_INET o AF_INET6
    hints.ai_socktype = this->type; // TCP o UDP
    char portstr[16];
    snprintf(portstr, sizeof(portstr), "%d", port);
    int st = getaddrinfo(hostip, portstr, &hints, &res);
    if (st != 0) {
        throw std::runtime_error("getaddrinfo failed: " + std::string(gai_strerror(st)));
    }
    int success = -1;
    for (rp = res; rp != nullptr; rp = rp->ai_next) {
        if (connect(this->idSocket, rp->ai_addr, rp->ai_addrlen) == 0) {
            success = 0;
            break;
        }
    }
    freeaddrinfo(res);
    if (success != 0) {
        throw std::runtime_error("Unable to connect to host (DNS)");
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
   // Llama a EstablishConnection con el puerto adecuado.
   return EstablishConnection(host, service == "http" ? 80 : 443);
}


/**
  * Bind method
  *    use "bind" Unix system call (man 3 bind) (server mode)
  *
  * @param      int port: bind a unamed socket to a port defined in sockaddr structure
  *
  *  Links the calling process to a service at port
  *
 **/
int VSocket::Bind( int port ) {
   struct sockaddr_in host4;  // Struct de metadata.
   
   host4.sin_family = AF_INET;   // Tipo IPv4.
   host4.sin_addr.s_addr = htonl( INADDR_ANY ); // Cualquier direccion, creo que por defecto es local.
   host4.sin_port = htons( port );  // Puerto.
   // Tanto htons como htonl, convierten los datos, en este caso ip y puerto, a orden de red.
   memset(host4.sin_zero, '\0', sizeof (host4.sin_zero));   // Memory allocation.

   if (::bind(this->idSocket, (struct sockaddr*)&host4, sizeof(host4))) {
      throw std::runtime_error("Bind failed: " + std::string(strerror(errno)));
   }
   
   return 0;

}


/**
  *  sendTo method
  *
  *  @param	const void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to send data
  *
  *  Send data to another network point (addr) without connection (Datagram)
  *
 **/
size_t VSocket::sendTo( const void * buffer, size_t size, void * addr ) {
   socklen_t addrlen;
   if (this->domain == AF_INET6) {
      addrlen = sizeof(sockaddr_in6);
   } else {
      addrlen = sizeof(sockaddr_in);
   }
   ssize_t sended_bytes = ::sendto(this->idSocket, buffer, size, 0, static_cast<const struct sockaddr *>(addr), addrlen);
   if (sended_bytes < 0) {
      throw std::runtime_error("sendTo failed: " + std::string(strerror(errno)));
   }
   return static_cast<size_t>(sended_bytes);

}


/**
  *  recvFrom method
  *
  *  @param void * buffer: data to send
  *  @param	size_t size data size to send
  *  @param	void * addr address to receive from data
  *
  *  @return	size_t bytes received
  *
  *  Receive data from another network point (addr) without connection (Datagram)
  *
 **/
size_t VSocket::recvFrom( void * buffer, size_t size, void * addr ) {
   socklen_t addrlen;
   if (this->domain == AF_INET6) {
      addrlen = sizeof(sockaddr_in6);
   } else {
      addrlen = sizeof(sockaddr_in);
   }
   ssize_t received_bytes = ::recvfrom(this->idSocket, buffer, size, 0, static_cast<struct sockaddr *>(addr), &addrlen);
   if (received_bytes < 0) {
      throw std::runtime_error("recvFrom failed: " + std::string(strerror(errno)));
   }
   return static_cast<size_t>(received_bytes);

}

