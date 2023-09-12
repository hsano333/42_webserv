#include "socket_writer.hpp"
#include <sys/types.h>
#include <sys/socket.h>

SocketWriter::SocketWriter()
{
}
SocketWriter::~SocketWriter()
{
}

int SocketWriter::write(FileDiscriptor fd, char const *buf, size_t size)
{
    return ::send(fd.to_int(), buf, size, MSG_DONTWAIT);
}
