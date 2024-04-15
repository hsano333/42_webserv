#include "webserv_timeout_event.hpp"
//#include "webserv_cleaner.hpp"

WebservTimeoutEvent::WebservTimeoutEvent()
{
    ;
}

WebservTimeoutEvent::~WebservTimeoutEvent()
{
    ;
}

WebservTimeoutEvent *WebservTimeoutEvent::singleton = NULL;
WebservTimeoutEvent *WebservTimeoutEvent::get_instance()
{
    if (WebservTimeoutEvent::singleton == NULL){
        singleton = new WebservTimeoutEvent();
    }
    return (singleton);
}


/*
bool clean_timeout_events(WebservTimeoutEvent *event, WebservEntity *entity)
{
    (void)entity;
    std::vector<WebservEvent *> timeout_events;

    event->cleaner()->clean_timeout_events();
    entity->set_completed(true);
    //for(size_t i=0;i<timeout_events.size();i++){
        //event->clean(timeout_events[i]->entity(), true);
    //}
    return (true);
}
*/

bool none(WebservTimeoutEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
    return (true);
}


WebservEvent *WebservTimeoutEvent::make(WebservEvent *event)
{
    DEBUG("WebservTimeoutEvent::make()");
    event->entity()->set_completed(true);
    WebservTimeoutEvent *timeout_event = WebservTimeoutEvent::get_instance();
    //WebservEvent *new_event = new WebservEvent( timeout_event, clean_timeout_events, NULL);
    WebservEvent *new_event = new WebservEvent( timeout_event, none, event->entity());
    return (new_event);
}

WebservEvent* WebservTimeoutEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    return (event_factory->make_clean_event(event, true));
}

E_EpollEvent WebservTimeoutEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
}

void WebservTimeoutEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
