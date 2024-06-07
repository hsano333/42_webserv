#ifndef SOCKET_H
#define SOCKET_H
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>
#include "port.hpp"
#include "file_discriptor.hpp"

typedef struct clientinfo {
    FileDiscriptor fd;
    char buf[1024];
    int n;
    int state;
} s_clientinfo;

class Socket
{
    public:
        Socket();
        Socket(Socket const &socket);
        ~Socket();
        Socket& operator=(Socket const &sock_fdet);
        static Socket create_from_port(Port const &port);
        int to_int();
        FileDiscriptor get_socket_fd() const;
        void close_fd();
    private:
        void init();
        FileDiscriptor sock_fd;
        Port port;
        void set_address_info(struct addrinfo& info) const;
        FileDiscriptor make_socket();
        const static int _SOCKET_NUM = 10;
};


#endif /* SOCKET_H */
