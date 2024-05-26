
#include "webserv_waiter.hpp"
#include "timeout_exception.hpp"
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
    io_multi_controller->wait(sec);
}

bool WebservWaiter::is_not_busy()
{
    DEBUG("WebservWaiter::is_not_busy()");

    size_t all_size = event_manager->event_size();
    size_t keep_alive_size = event_manager->keep_alive_event_size();
    return (all_size == keep_alive_size);
}

void WebservWaiter::fetch_events()
{
    DEBUG("WebservWaiter::fetch_event() event_size:" + Utility::to_string(event_manager->event_size()));

    int executable_event_size = io_multi_controller->executable_event_number();
    if(executable_event_size > 0){
        int event_size = io_multi_controller->executable_event_number();
        t_epoll_event *io_event = io_multi_controller->event_return_wrapper();
        for(int i=0;i<event_size;i++){
            MYINFO("WebservWaiter::fetch_event() event_manager->check_timeout():" + Utility::to_string(event_manager->check_timeout()));
            WebservEvent *event = this->event_factory->from_epoll_event(io_event[i]);
            if(event){
                event->update_time();
                event_manager->push(event);
            }
        }
    }


    std::set<WebservEvent *> killed_events;
    this->event_manager->retrieve_clean_events(killed_events);

    std::set<WebservEvent *>::iterator ite = killed_events.begin();
    std::set<WebservEvent *>::iterator end = killed_events.end();

    MYINFO("WebservWaiter::fetch_event() killed_events count:" + Utility::to_string(killed_events.size()));   //for(size_t i=0;i<timeout_events.size();i++){
    while(ite != end){
        WebservEvent *event;
        if((*ite)->entity()->event_error() == Timeout){
            DEBUG("DEBUG waiter check True timeout");
            //event = event_factory->make_timeout_event(*ite);
            Request *req = (*ite)->entity()->request();
            if((req && req->has_body() == false) || (req && req->read_completed())){
                // timeout in server process
                event = event_factory->make_event_from_http_error(*ite, "504");
            }else{
                // timeout in waiting request
                event = event_factory->make_event_from_http_error(*ite, "408");
            }
        }else if((*ite)->entity()->event_error() == DiedChild){
            DEBUG("DEBUG waiter check True died child");
            event = event_factory->make_event_from_http_error(*ite, "500");
        }
        event_manager->push(event);
        ite++;
    }
    return ;
}




