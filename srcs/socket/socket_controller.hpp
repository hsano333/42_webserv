#ifndef SOCKET_CONTROLLER
#define SOCKET_CONTROLLER
#include "socket.hpp"
#include "file_discriptor.hpp"

class SocketController
{
    public:
        SocketController();
        ~SocketController();
        FileDiscriptor accept_request(FileDiscriptor fd);
        FileDiscriptor accept_request(int fd);
        int read();
        int write();
    private:

};

#endif
