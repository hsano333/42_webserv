
#include "webserv_waiter.hpp"

WebservWaiter::WebservWaiter(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager
        ):
        io_multi_controller(io_multi_controller),
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
    io_multi_controller->wait();
}

bool WebservWaiter::is_not_busy()
{
    return (event_manager->event_size());
}

void WebservWaiter::copy_event_to_manager()
{
    //copy_event_to_manager(event_manager);
    //
}


WebservEvent* WebservWaiter::serve_event()
{
    std::vector<t_epoll_event> &take_out_events = io_multi_controller->take_out_event();
    for(size_t i=0;i<take_out_events.size();i++){
        WebservEvent *event = WebservEvent::from_epoll_event(take_out_events[i]);
        event_manager->push(event);
    }
    WebservEvent *returned_event = event_manager->top();
    event_manager->pop();
    return (returned_event);
}


