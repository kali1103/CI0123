#include <iostream>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <algorithm>

// Se crean los path de los respectivos pipes.
const char* client_to_fork = "/tmp/ctf";
const char* fork_to_server = "/tmp/fts";
const char* server_to_fork = "/tmp/stf";
const char* fork_to_client = "/tmp/ftc";

// Crea los pipes.
void fifafo () {
    mkfifo(client_to_fork, 0666);   // el primer 0 indica octal, luego, las
    mkfifo(fork_to_server, 0666);   // posiciones son owner-group-other, es una
    mkfifo(server_to_fork, 0666);   // suma de permisos: 
    mkfifo(fork_to_client, 0666);   // read = 4
                                    // write = 2
                                    // execute = 1
                                    // read + write = 6.
       
}

void server () {
    char buffer[256];
    int fdfs = open(fork_to_server, O_RDONLY);
    if (fdfs == -1) {
        perror("Open");
    }

    int n = read(fdfs, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("[Server] Recibido de fork: %s\n", buffer);
    }

    std::string response;

    // Protocolo personalizado
    if (strcmp(buffer, "TAKE menu") == 0) {
        // Listar archivos .txt del directorio actual
        DIR* dir = opendir(".");
        if (dir) {
            struct dirent* entry;
            std::vector<std::string> figuras;
            while ((entry = readdir(dir)) != nullptr) {
                std::string fname = entry->d_name;
                if (fname.length() > 4 && fname.substr(fname.length() - 4) == ".txt") {
                    figuras.push_back(fname);
                }
            }
            closedir(dir);
            std::sort(figuras.begin(), figuras.end());
            for (size_t i = 0; i < figuras.size(); ++i) {
                response += figuras[i];
                if (i + 1 < figuras.size()) response += ", ";
            }
            response += "\n";
        } else {
            response = "ERROR: No se pudo abrir el directorio\n";
        }
    } else if (strcmp(buffer, "TAKE ls") == 0) {
        // Listar todos los archivos y carpetas del directorio actual
        DIR* dir = opendir(".");
        if (dir) {
            struct dirent* entry;
            std::vector<std::string> items;
            while ((entry = readdir(dir)) != nullptr) {
                std::string fname = entry->d_name;
                if (fname != "." && fname != "..") {
                    items.push_back(fname);
                }
            }
            closedir(dir);
            std::sort(items.begin(), items.end());
            for (size_t i = 0; i < items.size(); ++i) {
                response += items[i];
                if (i + 1 < items.size()) response += ", ";
            }
            response += "\n";
        } else {
            response = "ERROR: No se pudo abrir el directorio\n";
        }
    } else {
        response = "ERROR: comando desconocido\n";
    }

    close(fdfs);

    int fdsf = open(server_to_fork, O_WRONLY);
    if (fdsf == -1) {
        perror("Open");
    }

    write(fdsf, response.c_str(), response.size());
    close(fdsf);
}

// Tenedor.
void forc () {
    int fdcf = open(client_to_fork, O_RDONLY); // Abre en modo lectura el fifo.
    if (fdcf == -1) {
        perror("Open");
    }

    char buffer[256];
    int n = read(fdcf, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0'; // Fin de string.
        printf("[Fork] Recibido de cliente: %s\n", buffer);
    }

    close(fdcf);

    // Traducir HTTP GET a protocolo TAKE.
    std::string comd;
    if (strstr(buffer, "GET /menu") != nullptr) {
        comd = "TAKE menu";
    } else if (strstr(buffer, "GET /dir") != nullptr) {
        comd = "TAKE ls";
    } else {
        comd = "UNKNOWN";
    }

    int fdfs = open(fork_to_server, O_WRONLY);
    if (fdfs == -1) {
        perror("Open");
    }

    write(fdfs, comd.c_str(), comd.size()); // Envia mensaje.
    close(fdfs);

    char buffer2[256];
    int fdsf_in = open(server_to_fork, O_RDONLY);
    if (fdsf_in == -1) {
        perror("Open");
    }

    int m = read(fdsf_in, buffer2, sizeof(buffer2) - 1);
    if (m > 0) {
        buffer2[m] = '\0';
        printf("[Fork] Recibido de server: %s", buffer2);
    }

    close(fdsf_in);

    // Enviar respuesta al cliente como HTTP.
    std::string resp = "HTTP/1.1 200 OK\nContent-Length: ";
    resp += std::to_string(strlen(buffer2));
    resp += "\n\n";
    resp += buffer2;

    int fdsf_out = open(fork_to_client, O_WRONLY);
    if (fdsf_out == -1) {
        perror("Open");
    }

    write(fdsf_out, resp.c_str(), resp.size()); // Envia mensaje.
    close(fdsf_out);

}

// Cliente.
void cliente () {
    std::string request = "GET /menu HTTP/1.1\n\n";
    int fdcf_out = open(client_to_fork, O_WRONLY);
    if (fdcf_out == -1) {
        perror("Open");
    }

    write(fdcf_out, request.c_str(), request.size());
    close(fdcf_out);

    char buffer[256];
    int fdcf_in = open(fork_to_client, O_RDONLY);
    if (fdcf_in == -1) {
        perror("Open");
    }

    int n = read(fdcf_in, buffer, sizeof(buffer) - 1);
    if (n > 0) {
        buffer[n] = '\0';
        printf("[Cliente] Recibido de fork: %s", buffer);
    }

    close(fdcf_in);
}

int main() {
    fifafo();

    pid_t pid_fork = fork();
    if (pid_fork == 0) {
        // Proceso tenedor.
        pid_t pid_server = fork();
        if (pid_server == 0) {
            // Proceso servidor.
            server();
        } else {
            // Proceso tenedor.
            forc();
        }
    } else {
        // Proceso cliente.
        sleep(1); // dar tiempo a que fork y server arranquen.
        cliente();
    }

    return 0;
}