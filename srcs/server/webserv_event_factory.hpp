#ifndef WEBSERV_EVENT_FACTORY_HPP
#define WEBSERV_EVENT_FACTORY_HPP
#include "socket_controller.hpp"
#include "socket_controller.hpp"
#include "webserv_event.hpp"
#include "io_multiplexing.hpp"

class WebservEventFactory
{
    public:
        WebservEventFactory(SocketController *socket_controller,
                FDManager *fd_manager,
                IOMultiplexing *io_multi_controller
                );
        ~WebservEventFactory();
        WebservEvent *from_epoll_event(t_epoll_event const &event);
        WebservEvent *make_application_event(FileDiscriptor fd, Request *req);
    private:
        SocketController *socket_controller;
        FDManager *fd_manager;
        IOMultiplexing *io_multi_controller;

};

#endif
