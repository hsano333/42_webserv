// #include "sock_fdet.hpp"
#include "tcp_socket.hpp"
#include "config.hpp"
#include <errno.h>
#include <iostream>
#include <string.h>
#include <unistd.h>

using std::cout;
using std::endl;

void TCPSocket::init()
{
    this->_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->_sock_fd < 0) {
        cout << strerror(errno) << endl;
        // throw std::exception();
        throw std::runtime_error("Failed to create sock_fdet\n");
    }

    struct addrinfo hint;
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;
    hint.ai_flags = AI_PASSIVE;
    hint.ai_socktype = SOCK_STREAM;
    struct addrinfo* res = NULL;
    int err = getaddrinfo(NULL, this->_port.c_str(), &hint, &res);
    if (err != 0) {
        this->close_fd();
        cout << "Error getaddrinfo() :" << gai_strerror(err) << endl;
        throw std::runtime_error("Failed to init()\n");
    }
    if (bind(this->_sock_fd, res->ai_addr, res->ai_addrlen) != 0) {
        this->close_fd();
        freeaddrinfo(res);
        cout << "Error bind:" << strerror(errno) << endl;
        throw std::runtime_error("Failed to bind()\n");
    }
    freeaddrinfo(res);
}

TCPSocket::TCPSocket()
{
    init();
}
TCPSocket::~TCPSocket()
{
    close_fd();
}
TCPSocket::TCPSocket(const TCPSocket& socket) : Socket(socket)
{
    init();
}
TCPSocket& TCPSocket::operator=(const TCPSocket& sock_fdet)
{
    if (&sock_fdet == this)
        return (*this);
    return (*this);
}
