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
    DEBUG("SocketReader::read() fd=" + fd.to_string());
    return ::recv(fd.to_int(), buf, size, MSG_DONTWAIT);
}
