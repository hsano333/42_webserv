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
    std::cout << "socket::write() size:" << size << std::endl;
    //std::cout << "socket::write():" << buf << std::endl;
    //std::cout << "socket::write():" << fd.to_int() << std::endl;
    try{
        int result =  ::send(fd.to_int(), buf, size, MSG_DONTWAIT);
        std::cout << "socket::result:" << result << std::endl;
        return (result);
    }catch(...){
        std::cout << "TEST" << std::endl;
    }
        return (-1);
    //return ::send(fd.to_int(), buf, size, MSG_DONTWAIT);
}
