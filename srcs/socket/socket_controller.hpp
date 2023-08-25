#ifndef SOCKET_CONTROLLER
#define SOCKET_CONTROLLER
#include "socket.hpp"
#include "file_discriptor.hpp"

class SocketController
{
    public:
        SocketController();
        ~SocketController();
        int accept_request(FileDiscriptor fd)
        int read();
        int write();
    private:

};

#endif
