#ifndef SOCKET_FACTORY_HPP
#define SOCKET_FACTORY_HPP
#include "config.hpp"
#include "socket_manager.hpp"
#include "fd_manager.hpp"

class SocketFactory
{
    public:
        SocketFactory(FDManager *fd_manager);
        ~SocketFactory();
        //SocketManager *create(Config *cfg);
        SocketRepository *create_from_config(Config *cfg);
    private:
        SocketRepository *create_socket_repository(Config *cfg);
        FDManager *fd_manager;
        //Config const *cfg;
        //SocketRepository *socket_repository;
};

#endif
