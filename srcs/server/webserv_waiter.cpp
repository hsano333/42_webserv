
#include "webserv_waiter.hpp"

WebservWaiter::WebservWaiter(
            IOMultiplexing *io_controller,
            EventManager event_manager
        ):
        io_controller(io_controller),
        event_manager(event_manager)
{
    ;
}

WebservWaiter::~WebservWaiter()
{
    ;
}

void WebservWaiter::wait()
{
    io_controller->wait();
}

WebservEvent* WebservWaiter::place_event()
{
    WebservEvent *event = new WebservEvent();
    return event;
}
