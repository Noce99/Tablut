#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <string.h>

#define PORT_WHITE 5800
#define PORT_BLACK 5801

class Socket {

    private:
        int fd;

    public:
        Socket(bool, std::string);
        int get_fd();
        void send_to_server(std::string);
        std::vector<std::vector<char>> recive_from_server();
};

#endif
