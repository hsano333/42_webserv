#include "socket_manager.hpp"

SocketManager::SocketManager() : 
                socket_base_repository(NULL),
                socket_rw_repository(NULL)
{
    ;
}

SocketManager::~SocketManager()
{
    delete (socket_base_repository);
    delete (socket_rw_repository);
}

/*
void SocketManager::set_auto_pointer()
{
    base_ptr(this->socket_base_repository);
    rw_ptr(this->socket_rw_repository);
}
*/

void SocketManager::set_base_repository(SocketRepository *base)
{
    this->socket_base_repository = base;
}

void SocketManager::set_rw_repository(SocketRepository *rw)
{
    this->socket_rw_repository = rw;
}

size_t SocketManager::get_base_sockets_size()
{
    return (this->socket_base_repository->size());
}

std::map<FileDiscriptor, Socket>::const_iterator SocketManager::base_begin()
{
    return (this->socket_base_repository->begin());
}

std::map<FileDiscriptor, Socket>::const_iterator SocketManager::base_end()
{
    return (this->socket_base_repository->end());
}
