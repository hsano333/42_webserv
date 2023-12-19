#ifndef EVENT_CONTROLLER_HPP
#define EVENT_CONTROLLER_HPP
#include "event_manager.hpp"
#include "webserv_event.hpp"

class EventController
{
    public:
        EventController(
                EventManager *event_manager,
                IOMultiplexing *io_multi_controller,
                WebservEventFactory *event_factory,
                FDManager *fd_manager
                );
        ~EventController();
        //void restart_communication(WebservEvent *event);
        void change_write_event(WebservEvent *event);
        void next_event(WebservEvent *event);
    private:
        EventManager        *event_manager;
        IOMultiplexing *io_multi_controller;
        WebservEventFactory *event_factory;
        FDManager *fd_manager;

        void set_next_epoll_event(WebservEvent *event, WebservEvent *next_event);
};
#endif
