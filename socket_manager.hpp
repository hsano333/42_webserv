#ifndef SOCKET_MANAGER_CPP
#define SOCKET_MANAGER_CPP
#include "socket_repository"

class SocketManager
{
    public:
        SocketManager();
        ~SocketManager();
    private:
        SocketRepository *socket_repository;
        //std::map<int, Socket*> socket_repository;
};

#endif
