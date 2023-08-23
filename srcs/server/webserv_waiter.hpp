
#ifndef WEBSERV_DISPATCHER_HPP
#define WEBSERV_DISPATCHER_HPP
#include "webserv_event.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"

/*
typedef enum E_WebservEvent
{
    READ_EVENT,
    APPLICATION_EVENT,
    WRITE_EVENT,
} WebservEvent;
*/

class WebservWaiter
{
    public:
        WebservWaiter(IOMultiplexing *io_controller,
                      EventManager event_manager
                );
        ~WebservWaiter();
        void wait();
        WebservEvent* place_event();
    private:
        IOMultiplexing *io_controller;
        EventManager &event_manager;
        //WebservEvent *event;
};

#endif
