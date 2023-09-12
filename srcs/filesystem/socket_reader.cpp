#include "socket_reader.hpp"
#include "global.hpp"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

SocketReader::SocketReader()
{
    DEBUG("SocketReader::SocketReader()");
}
SocketReader::~SocketReader()
{
}

int SocketReader::read(FileDiscriptor fd, char *buf, size_t size)
{
    DEBUG("SocketReader::read()");
    std::cout << "fd:" << fd.to_string() << std::endl;
    std::cout << "size:" << size << std::endl;
    return ::recv(fd.to_int(), buf, size, MSG_DONTWAIT);
}
