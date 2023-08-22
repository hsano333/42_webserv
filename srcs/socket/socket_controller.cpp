#include "socket_controller.hpp"
#include "utility.hpp"
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>


SocketController::SocketController()
{
;
}

SocketController::~SocketController()
{
;
}

int SocketController::accept_request(int fd)
{
    struct sockaddr_in client;
    Utility::memset(&client, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(client);

    int fd = accept(socket->_sock_fd, (struct sockaddr*)&client, &len);
    if (fd < 0) {
        ERROR("Error socket accept() fd:" + Utility::to_string(socket->_sock_fd));
        throw std::runtime_error("Error socket:failure to accept()");
    }

    /*
    FDManager* fd_m = new FDManager(fd);
    this->_fd_map.insert(std::make_pair(fd, fd_m));
    */
    int cur_flags = fcntl(fd, F_GETFL, 0);
    cur_flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, cur_flags);
    return (fd);
}
