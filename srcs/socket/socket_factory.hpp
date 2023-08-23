#ifndef SOCKET_FACTORY_HPP
#define SOCKET_FACTORY_HPP
#include "config.hpp"
#include "socket_manager.hpp"

class SocketFactory
{
    public:
        SocketFactory();
        ~SocketFactory();
        //SocketManager *create(Config *cfg);
        SocketRepository *create_from_config(Config *cfg);
    private:
        SocketRepository *create_socket_repository(Config *cfg);
        //Config const *cfg;
        //SocketRepository *socket_repository;
};

#endif
