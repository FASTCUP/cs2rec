#include "cmdserver.hpp"
#include "../util/defer.hpp"
#include <cstdio> // std::printf, std::remove
#include <cstdlib> // std::system
#include <cstring> // str::strcpy
#include <unistd.h> // close
#include <sys/socket.h> // socket
#include <sys/un.h> // sockaddr_un
#include <cerrno>
#include <string>

int main() {
    // Remove the socket file, which remains if the server is terminated
    std::remove(CmdServer::ADDRESS);

    int server_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::printf("Failed to create server socket\n");
        return -1;
    }
    defer { close(server_socket); };

    sockaddr_un address;
    address.sun_family = AF_LOCAL;
    std::strcpy(address.sun_path, CmdServer::ADDRESS);

    if (bind(server_socket, (sockaddr*)&address, sizeof(address)) == -1) {
        std::printf("Failed to bind server socket\n");
        return -1;
    }
    defer { unlink(CmdServer::ADDRESS); };

    if (listen(server_socket, CmdServer::BACKLOG) == -1) {
        std::printf("Failed to listen on server socket\n");
        return -1;
    }
    
    while (true) {
        int client_socket = accept(server_socket, nullptr, nullptr);
        if (client_socket == -1 && (errno == EBADF || errno == EINVAL || errno == ENOTSOCK))
            break; // The socket APIs are being used incorrectly
        
        defer { close(client_socket); };

        std::string command;
        bool failure = false;

        while (true) {
            char buffer[256];
            ssize_t count = recv(client_socket, buffer, sizeof(buffer), 0);
            if (count <= 0) {
                failure = count == -1;
                break;
            }
            command.append(buffer, count);
        }

        if (failure)
            break;

        // NOTE(Cade): This will block the server until the command finishes.
        //             The client should consider appending " &" to run the command in the background.
        std::system(command.c_str());
    }

    return 0;
}