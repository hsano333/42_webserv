#ifndef FD_MANAGER_H
#define FD_MANAGER_H

#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"

class FDManager
{
  public:
    FDManager();
    ~FDManager();
    void add_socket(FileDiscriptor fd);
    void close_socket(FileDiscriptor fd);
    void add_socket_and_epoll_fd(FileDiscriptor io_fd, FileDiscriptor socket_fd);
    void close_fd(FileDiscriptor fd);
    FileDiscriptor socket_fd_from_epoll_fd(FileDiscriptor epoll_fd);
    bool is_registered(FileDiscriptor epoll_fd);
    FileDiscriptor get_sockfd(FileDiscriptor fd);


  private:
    std::vector< FileDiscriptor> fd_of_sockets;

    // key is epoll fd and value is socket fd
    std::map<FileDiscriptor, FileDiscriptor> epoll_fd_and_socket;

    void close_all();
};
#endif /* FD_MANAGER_H */
