#include "config.hpp"
#include "string.h"
#include "utility.hpp"
#include "tcp_socket.hpp"
#include "global.hpp"
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

using std::cout;
using std::endl;
using std::cerr;

Socket::Socket() : sock_fd(FileDiscriptor::from_int(0))
{
    ;
}

Socket::Socket(Socket const &socket)
{
    this->sock_fd = socket.sock_fd;
    this->port = socket.port;
}

Socket::~Socket()
{
}

Socket Socket::create_from_port(Port const &port_)
{
    MYINFO("Socket::create_from_port port:" + port_.to_string());
    Socket socket;
    socket.port = port_;
    socket.init();
    return socket;
}


FileDiscriptor Socket::make_socket()
{
    int tmp_fd = (socket(AF_INET, SOCK_STREAM, 0));
    return (FileDiscriptor::from_int(tmp_fd));
}

void Socket::set_address_info(struct addrinfo& info) const
{
    info.ai_family = AF_INET;
    info.ai_flags = AI_PASSIVE;
    info.ai_socktype = SOCK_STREAM;
}

void Socket::init()
{
    this->sock_fd = this->make_socket();
    MYINFO("Socket::init() sock_fd:" + Utility::to_string(this->sock_fd));

    struct addrinfo hint;
    Utility::memset(&hint, 0, sizeof(struct addrinfo));
    this->set_address_info(hint);
    struct addrinfo* res = NULL;
    int err = getaddrinfo(NULL, this->port.c_str(), &hint, &res);
    if (err != 0) {
        this->close_fd();
        std::string str = gai_strerror(err);
        ERROR("Error getaddrinfo() :" + str);
        throw std::runtime_error("Failed to init()\n");
    }


    int yes = 1;
    setsockopt(this->sock_fd.to_int(), SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));
    setsockopt(this->sock_fd.to_int(), SOL_SOCKET, SO_KEEPALIVE, (const char*)&yes, sizeof(yes));

    if (bind(this->sock_fd.to_int(), res->ai_addr, res->ai_addrlen) != 0) {
        this->close_fd();
        freeaddrinfo(res);
        ERROR("Error bind:");
        throw std::runtime_error("Error:Failed to bind() exit webserv \n");
    }
    freeaddrinfo(res);
    listen(this->sock_fd.to_int(), _SOCKET_NUM);

}

Socket& Socket::operator=(const Socket& sock_fdet)
{
    if (&sock_fdet == this)
        return (*this);
    return (*this);
}

FileDiscriptor Socket::get_socket_fd() const
{
    return (this->sock_fd);
}

void Socket::close_fd()
{
    this->sock_fd.close();
}

int Socket::to_int()
{
    return (this->sock_fd.to_int());
}

