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
    //delete (singleton);
}


SocketReader* SocketReader::singleton = NULL;
SocketReader *SocketReader::get_instance()
{
    if (SocketReader::singleton == NULL){
        singleton = new SocketReader();
    }
    return (singleton);
}


int SocketReader::read(FileDiscriptor fd, char *buf, size_t size, std::fstream *ifs)
{
    (void)ifs;
    DEBUG("SocketReader::read()");
    //std::cout << "fd:" << fd.to_string() << std::endl;
    //std::cout << "size:" << size << std::endl;
    return ::recv(fd.to_int(), buf, size, MSG_DONTWAIT);
    //return ::recv(fd.to_int(), buf, size, 0);
}
