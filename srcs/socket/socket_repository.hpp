#ifndef SOCKET_REPOSITORY_HPP
#define SOCKET_REPOSITORY_HPP

#include "socket.hpp"
class SocketRepository
{
    public:
        SocketRepository();
        ~SocketRepository();
        void insert(Socket socket);
        void close_all();
        Socket get(int id);
        size_t size();
    public:
        std::map<int, Socket> sockets;
};

#endif
