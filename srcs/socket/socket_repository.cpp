
#include "socket_repository.hpp"

SocketRepository::SocketRepository()
{
    ;
}
SocketRepository::~SocketRepository()
{
    this->close_all();
}

void SocketRepository::insert(Socket socket)
{
    FileDiscriptor fd = socket.get_socket_fd();
    this->sockets.insert(std::make_pair(fd, socket));
}

void SocketRepository::close_all()
{
    std::map<FileDiscriptor, Socket>::iterator ite = this->sockets.begin();
    std::map<FileDiscriptor, Socket>::iterator end = this->sockets.end();

    while(ite != end){
        ite->first.close();
    }
    //sockets.clear();
}

size_t SocketRepository::size()
{
    return (this->sockets.size());
}

std::map<FileDiscriptor, Socket>::const_iterator SocketRepository::begin()
{
    return (this->sockets.begin());
}

std::map<FileDiscriptor, Socket>::const_iterator SocketRepository::end()
{
    return (this->sockets.end());
}
