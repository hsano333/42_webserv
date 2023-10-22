#ifndef EVENT_CONTROLLER_HPP
#define EVENT_CONTROLLER_HPP
#include "event_manager.hpp"
#include "webserv_event.hpp"

class EventController
{
    public:
        EventController(
                EventManager *event_manager,
                IOMultiplexing *io_multi_controller
                );
        ~EventController();
        void restart_communication(WebservEvent *event);
        void change_write_event(WebservEvent *event);
    private:
        EventManager        *event_manager;
        IOMultiplexing *io_multi_controller;
};
#endif
