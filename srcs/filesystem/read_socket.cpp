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

int ReadSocket::iread(int fd, char *buf)
{
    return recv(fd, buf, MAX_BUF, MSG_DONTWAIT);
}
