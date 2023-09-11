#include "read_socket.hpp"
#include "global.hpp"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

ReadSocket::ReadSocket()
{
    DEBUG("ReadSocket::ReadSocket()");
}
ReadSocket::~ReadSocket()
{
}

int ReadSocket::read(FileDiscriptor fd, char *buf, size_t size)
{
    DEBUG("ReadSocket::read()");
    std::cout << "fd:" << fd.to_string() << std::endl;
    std::cout << "size:" << size << std::endl;
    return ::recv(fd.to_int(), buf, size, MSG_DONTWAIT);
}
