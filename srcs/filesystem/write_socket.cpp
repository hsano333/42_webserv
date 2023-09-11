#include "write_socket.hpp"
#include <sys/types.h>
#include <sys/socket.h>

WriteSocket::WriteSocket()
{
}
WriteSocket::~WriteSocket()
{
}

int WriteSocket::write(FileDiscriptor fd, char const *buf, size_t size)
{
    return ::send(fd.to_int(), buf, size, MSG_DONTWAIT);
}
