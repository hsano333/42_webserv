#include "read_socket.hpp"
#include "global.hpp"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>

ReadSocket::ReadSocket()
{
}
ReadSocket::~ReadSocket()
{
}

int ReadSocket::read(FileDiscriptor fd, char *buf, size_t size)
{
    return recv(fd.to_int(), buf, size, MSG_DONTWAIT);
}
