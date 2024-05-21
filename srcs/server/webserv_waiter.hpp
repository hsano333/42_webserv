
#ifndef WEBSERV_DISPATCHER_HPP
#define WEBSERV_DISPATCHER_HPP
#include "webserv_event.hpp"
#include "webserv_event_factory.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"
#include "webserv_timeout_event.hpp"


class WebservWaiter
{
    public:
        WebservWaiter(IOMultiplexing *io_multi_controller,
                      EventManager *event_manager,
                      WebservEventFactory *event_factory
                );
        ~WebservWaiter();
        void wait(int sec);
        bool is_not_busy();
        void fetch_events();
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        WebservEventFactory *event_factory;
};

#endif
