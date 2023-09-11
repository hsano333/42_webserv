#include "socket_controller.hpp"
#include "utility.hpp"
#include "global.hpp"
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

FileDiscriptor SocketController::accept_request(FileDiscriptor sock_fd)
{
    struct sockaddr_in client;
    Utility::memset(&client, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(client);

    int tmp_fd = accept(sock_fd.to_int(), (struct sockaddr*)&client, &len);
    MYINFO("SocketController::accept_request fd:" + Utility::to_string(sock_fd.to_int()) + ", new fd:" + Utility::to_string(tmp_fd));
    FileDiscriptor fd = FileDiscriptor::from_int(tmp_fd);
    
    /*
    if (fd < 0) {
        ERROR("Error socket accept() fd:" + Utility::to_string(sock_fd.to_int()));
        throw std::runtime_error("Error socket:failure to accept()");
    }
    */

    /*
    FDManager* fd_m = new FDManager(fd);
    this->_fd_map.insert(std::make_pair(fd, fd_m));
    */
    int cur_flags = fcntl(fd.to_int(), F_GETFL, 0);
    cur_flags |= O_NONBLOCK;
    fcntl(fd.to_int(), F_SETFL, cur_flags);
    return (fd);
}

FileDiscriptor SocketController::accept_request(int tmp_fd)
{
    FileDiscriptor fd = FileDiscriptor::from_int(tmp_fd);
    return (accept_request(fd));

}

