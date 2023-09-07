#ifndef WEBSERV_EVENT_FACTORY_HPP
#define WEBSERV_EVENT_FACTORY_HPP
#include "socket_controller.hpp"
#include "webserv_event.hpp"

class WebservEventFactory
{
    public:
        WebservEventFactory(SocketController *socket_controller);
        ~WebservEventFactory();
        WebservEvent *from_epoll_event(t_epoll_event const &event);
    private:
        SocketController *socket_controller;

};

#endif
