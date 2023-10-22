#ifndef EVENT_CONTROLLER_HPP
#define EVENT_CONTROLLER_HPP
#include "event_manager.hpp"
#include "webserv_event.hpp"

class EventController
{
    public:
        EventController(EventManager *event_manager);
        ~EventController();
        void restart_communication(WebservEvent *event);
    private:
        EventManager        *event_manager;
};
#endif
