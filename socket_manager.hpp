#ifndef SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP
#define SOCKET_MANAGER_HPP
#include "socket.hpp"
#include "socket_repository.hpp"
#include "response.hpp"
#include <map>
#include <vector>
#include <memory>

class SocketManager
{
    public:
        SocketManager();
        ~SocketManager();
        void set_base_repository(SocketRepository *base);
        void set_rw_repository(SocketRepository *rw);
        size_t get_base_sockets_size();
        std::map<FileDiscriptor, Socket>::const_iterator base_begin();
        std::map<FileDiscriptor, Socket>::const_iterator base_end();
        //void set_auto_pointer();
    private:
        SocketRepository *socket_base_repository;
        SocketRepository *socket_rw_repository;
        //std::auto_ptr<SocketRepository> base_ptr;
        //std::auto_ptr<SocketRepository> rw_ptr;
        //std::map<int, Socket*> sockets;
        //std::map<std::pair<Socket*, int>, Response*> responses;
};

#endif
