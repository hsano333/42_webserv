#ifndef SOCKET_FACTORY_HPP
#define SOCKET_FACTORY_HPP
#include "config.hpp"
#include "fd_manager.hpp"

class SocketFactory
{
    public:
        SocketFactory(FDManager *fd_manager);
        ~SocketFactory();
        SocketRepository *create_from_config(Config *cfg);
    private:
        SocketRepository *create_socket_repository(Config *cfg);
        FDManager *fd_manager;
};

#endif
