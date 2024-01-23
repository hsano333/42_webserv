#include "socket_writer.hpp"
#include "connection_exception.hpp"
#include "global.hpp"
#include <sys/types.h>
#include <sys/socket.h>

SocketWriter::SocketWriter()
{
}

SocketWriter::~SocketWriter()
{
    //delete (singleton);
}

SocketWriter* SocketWriter::singleton = NULL;
SocketWriter *SocketWriter::get_instance()
{
    if (SocketWriter::singleton == NULL){
        singleton = new SocketWriter();
    }
    return (singleton);
}

int SocketWriter::write(FileDiscriptor fd, char const *buf, size_t size, std::fstream *ifs)
{
    (void)ifs;
    std::cout << "socket::write() size:" << size << std::endl;
    ssize_t read_size = ::send(fd.to_int(), buf, size, MSG_DONTWAIT | MSG_NOSIGNAL);
    if(read_size == 0){
        ERROR("Client Write Close");
        throw ConnectionException("Client Write Close");
    }
    return (read_size);
}
