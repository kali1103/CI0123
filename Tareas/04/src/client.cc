/*
 *  Cliente, socket, IPv4 e IPv6.
 *
 */

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <vector>
#include <regex>
#include "VSocket.h"
#include "Socket.h"
#include "SSLSocket.h"
#define buffersize 256


int main( int argc, char * argv[] ) {
  // Diccionario con las direcciones posibles.
  std::map<std::string, std::string> dicc = {
    {"os",  "os.ecci.ucr.ac.cr"}, // -> DNS.
    {"osi", "10.84.166.62"},      // -> IP local.
    {"ose", "163.178.104.62"}     // -> IP Publica.
  };

  const char * direccion = dicc["ose"].c_str(); // Direccion por defecto.
  const char * solicitud;
  std::string request;
  std::string figure = "whale-1.txt"; // Figura por defecto.
  bool ipv6 = false;
  bool ssl = false;

  // Analiza los argumentos.
  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];
    if (arg == "ipv6") {
      ipv6 = true;
    } else if (arg == "ipv4") {
      ipv6 = false;
    } else if (arg == "ssl") {
      ssl = true;
    } else if (arg == "nossl") {
      ssl = false;
    } else if (dicc.count(arg)) {
      direccion = dicc[arg].c_str();
    } else if (arg.size() > 4 && arg.substr(arg.size() - 4) == ".txt") {
      figure = arg;
    } else {
      std::cout << "Argumento no reconocido: " << arg << std::endl;
      return 1;
    }
  }
  if (!direccion) {
    direccion = dicc["ose"].c_str();
  }

  // Ensamblaje de la solicitud HTTP.
  std::string p0 = "\r\n"
  "GET /aArt/index.php?disk=Disk-01&fig=";
  std::string p1 = " HTTP/1.1\r\n"
  "Host: os.ecci.ucr.ac.cr\r\n"
  "Connection: close\r\n"
  "\r\n";

  request = p0 + figure + p1;
  solicitud = request.c_str();
  

  std::cout << "Se utiliza la siguiente direccion: " << direccion << "\n";
  std::cout << "IPv6: " << (ipv6 ? "Si" : "No") << "\n";
  std::cout << "SSL: " << (ssl ? "Si" : "No") << "\n";

  std::cout << solicitud << "\n";

  VSocket * s;	
  int port = ssl ? 443 : 80;

  if (ssl) {
    s = new SSLSocket(ipv6);	// Crea un nuevo socket SSL.
  } else {
    s = new Socket( 's', ipv6 ); // Crea un nuevo socket comun.
  }
  
  s->MakeConnection( direccion, port);
  s->Write(solicitud, strlen(solicitud));

  char draw[buffersize];
  std::string reading;
  int breads; // -> Bytes leidos.
  do {
    breads = s->Read(draw, buffersize - 1); // Lee hasta buffersize - 1 bytes.
    if (breads > 0) {
      reading.append(draw, breads); // Hace un append de lo que se ha leido.
      printf("%d bytes leidos\n", breads);
    }
    
  } while (breads > 0);
  memset(draw, 0 , buffersize);
  size_t start = reading.find("<PRE>");
  size_t end   = reading.find("</PRE>");

  // Elimina las etiquetas del dibujo.
  if (start != std::string::npos && end != std::string::npos && end > start) {
    std::string asciiArt = reading.substr(start + 5, end - (start + 5));
    std::cout << asciiArt << std::endl;
  } else {
    std::cout << "Elemento no disponible." << "\n";
  }
  fflush(stdout);
}
