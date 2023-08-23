
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
    int fd = socket.get_socket_fd();
    this->sockets.insert(std::make_pair(fd, socket));
}

void SocketRepository::close_all()
{
    std::map<int, Socket>::iterator ite = this->sockets.begin();
    std::map<int, Socket>::iterator end = this->sockets.end();

    while(ite != end){
        close (ite->first);
    }
    //sockets.clear();
}

size_t SocketRepository::size()
{
    return (this->sockets.size());
}

std::map<int, Socket>::const_iterator SocketRepository::begin()
{
    return (this->sockets.begin());
}

std::map<int, Socket>::const_iterator SocketRepository::end()
{
    return (this->sockets.end());
}
