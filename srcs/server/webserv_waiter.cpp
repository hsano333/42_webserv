
#include "webserv_waiter.hpp"
#include "timeout_exception.hpp"
#include "webserv_timeout_event.hpp"
#include "webserv_nothing_event.hpp"

WebservWaiter::WebservWaiter(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager,
            WebservEventFactory *event_factory
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager),
        event_factory(event_factory)
{
    ;
}

WebservWaiter::~WebservWaiter()
{
    ;
}



void WebservWaiter::wait()
{
    try{
        io_multi_controller->wait();
    }catch(TimeoutException &e){
        MYINFO("waiter catch epoll timeout");
        int count = Utility::to_int(e.what());
        event_manager->count_up_to_all_event(count);
    }
}

bool WebservWaiter::is_not_busy()
{
    return (event_manager->event_size() == 0);
}

void WebservWaiter::copy_event_to_manager()
{
    //copy_event_to_manager(event_manager);
    //
}


WebservEvent* WebservWaiter::serve_event()
{
    DEBUG("WebservWaiter::serve_event() event_size:" + Utility::to_string(event_manager->event_size()));
    if(event_manager->event_size() > 0){
        return (event_manager->pop_first());
    }
    int executable_event_size = io_multi_controller->executable_event_number();
    MYINFO("WebservWaiter::serve_event() executable_event_size:" + Utility::to_string(executable_event_size));
    if(executable_event_size > 0){
        std::vector<t_epoll_event> &io_event = io_multi_controller->take_out_event();
        for(int i=0;i<executable_event_size;i++){
            WebservEvent *event = this->event_factory->from_epoll_event(io_event[i]);
            event_manager->push(event);
        }
    }
    /*
    std::vector<t_epoll_event> &take_out_events = io_multi_controller->take_out_event();
    for(size_t i=0;i<take_out_events.size();i++){
        WebservEvent *event = WebservEvent::from_epoll_event(take_out_events[i]);
        event_manager->push(event);
    }
    */
    if(event_manager->event_size() > 0){
        WebservEvent *returned_event = event_manager->pop_first();
        return (returned_event);
    }

    if(event_manager->check_timeout()){
        return (new WebservTimeoutEvent());
    }
    return (new WebservNothingEvent);
    //return (NULL);
}


