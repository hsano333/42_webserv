#ifndef SOCKET_CONTROLLER
#define SOCKET_CONTROLLER
#include "socket.hpp"

class SocketController
{
    public:
        SocketController();
        ~SocketController();
        int accept_request(int fd)
        int read();
        int write();
    private:

};

#endif
