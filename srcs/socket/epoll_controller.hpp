#ifndef EPOLL_CONTROLLER_HPP
#define EPOLL_CONTROLLER_HPP

#include <vector>
#include <map>
#include <sys/epoll.h>
#include "epoll.hpp"
#include "event_manager.hpp"
#include "io_multiplexing.hpp"
#include "socket_controller.hpp"

class EpollController : public IOMultiplexing
{
    public:
        EpollController(Epoll epoll, SocketRepository *socket_repository, SocketController *socket_controller, FDManager *fd_manager);
        ~EpollController();
        void wait(int msec);
        void add(FileDiscriptor const &fd, uint32_t event);
        void erase(FileDiscriptor const &fd);
        void modify(FileDiscriptor const &fd, uint32_t event);
        FileDiscriptor const &fd();
        int executable_event_number();
        void init_epoll();
    private:
        Epoll epoll;
        SocketRepository *socket_repository;
        SocketController *socket_controller;
        EventManager *event_manager;
        FDManager *fd_manager;
        t_epoll_event *event_return_wrapper();

    private:
};

#endif
