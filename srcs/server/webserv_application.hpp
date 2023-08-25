#ifndef WEBSERV_APPLICATION_HPP
#define WEBSERV_APPLICATION_HPP
#include "webserv_event.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"

class WebservApplication
{
    public:
        WebservApplication(IOMultiplexing *io_multi_controller,
                      EventManager *event_manager
                );
        ~WebservApplication();
        void process(WebservEvent *event);
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
};

#endif
