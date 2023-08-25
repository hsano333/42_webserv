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
        //Socket get(int id);
        //std::map<int, Socket> const & socket();
        size_t size();
        std::map<FileDiscriptor, Socket>::const_iterator begin();
        std::map<FileDiscriptor, Socket>::const_iterator end();
    private:
        std::map<FileDiscriptor, Socket> sockets;
};

#endif
