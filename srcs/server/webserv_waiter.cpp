
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



void WebservWaiter::wait(int sec)
{
    try{
        io_multi_controller->wait(sec);
    }catch(TimeoutException &e){
        MYINFO("waiter catch epoll timeout");
        int count = Utility::to_int(e.what());
        event_manager->count_up_to_all_event(count);
    }
}

bool WebservWaiter::is_not_busy()
{
    size_t all_size = event_manager->event_size();
    size_t keep_alive_size = event_manager->keep_alive_event_size();
    cout << "all_size:" << all_size << endl;
    cout << "keep_alive_size:" << keep_alive_size << endl;
    return (all_size == keep_alive_size);
}

void WebservWaiter::copy_event_to_manager()
{
    //copy_event_to_manager(event_manager);
    //
}


/*
WebservEvent* WebservWaiter::fetch_event()
{
    DEBUG("WebservWaiter::fetch_event() event_size:" + Utility::to_string(event_manager->event_size()));

    if(event_manager->event_size() > 0){
        return (event_manager->pop_first());
    }
    int executable_event_size = io_multi_controller->executable_event_number();
    MYINFO("WebservWaiter::fetch_event() executable_event_size:" + Utility::to_string(executable_event_size));
    if(executable_event_size > 0){
        std::vector<t_epoll_event> &io_event = io_multi_controller->take_out_event();
        for(int i=0;i<executable_event_size;i++){
            WebservEvent *event = this->event_factory->from_epoll_event(io_event[i]);
            event_manager->push(event);
        }
    }
    MYINFO("No.1 WebservWaiter::fetch_event() event_manager->event_size():" + Utility::to_string(event_manager->event_size()));
    if(event_manager->event_size() > 0){
        WebservEvent *returned_event = event_manager->pop_first();
        MYINFO("No.2 WebservWaiter::fetch_event() event_manager->event_size():" + Utility::to_string(event_manager->event_size()));
        printf("returned_event=%p\n", returned_event);
        return (returned_event);
    }

    if(event_manager->check_timeout()){
        MYINFO("WebservWaiter::fetch_event() event_manager->check_timeout():" + Utility::to_string(event_manager->check_timeout()));
        return (new WebservTimeoutEvent());
    }
    MYINFO("WebservWaiter::fetch_event() return new WebservNothingEvent():");
    return (new WebservNothingEvent);
    //return (NULL);
}
*/

void WebservWaiter::fetch_events()
{
    DEBUG("WebservWaiter::fetch_event() event_size:" + Utility::to_string(event_manager->event_size()));

    int executable_event_size = io_multi_controller->executable_event_number();
    MYINFO("WebservWaiter::fetch_event() executable_event_size:" + Utility::to_string(executable_event_size));
    if(executable_event_size > 0){
        std::vector<t_epoll_event> &io_event = io_multi_controller->take_out_event();
        for(int i=0;i<executable_event_size;i++){
            WebservEvent *event = this->event_factory->from_epoll_event(io_event[i]);
            if(event){
                event_manager->push(event);
            }
        }
    }
    MYINFO("No.1 WebservWaiter::fetch_event() event_manager->event_size():" + Utility::to_string(event_manager->event_size()));
    if(event_manager->event_size() > 0){
        //WebservEvent *returned_event = event_manager->pop_first();
        //MYINFO("No.2 WebservWaiter::fetch_event() event_manager->event_size():" + Utility::to_string(event_manager->event_size()));
        //printf("returned_event=%p\n", returned_event);
        //event_manager->push(event);
        //return (events);
        return ;
    }

    if(event_manager->check_timeout()){
        MYINFO("WebservWaiter::fetch_event() event_manager->check_timeout():" + Utility::to_string(event_manager->check_timeout()));
        event_manager->push(WebservTimeoutEvent::make());
    }
    MYINFO("WebservWaiter::fetch_event() return new WebservNothingEvent():");
    //return (new WebservNothingEvent);
    //return (NULL);
    return ;
}




