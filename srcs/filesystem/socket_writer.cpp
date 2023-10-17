#include "socket_writer.hpp"
#include <sys/types.h>
#include <sys/socket.h>

SocketWriter::SocketWriter()
{
}

SocketWriter::~SocketWriter()
{
    delete (singleton);
}

SocketWriter* SocketWriter::singleton = NULL;
SocketWriter *SocketWriter::get_instance()
{
    if (SocketWriter::singleton == NULL){
        singleton = new SocketWriter();
    }
    return (singleton);
}

int SocketWriter::write(FileDiscriptor fd, char const *buf, size_t size)
{
    return ::send(fd.to_int(), buf, size, MSG_DONTWAIT);
}
