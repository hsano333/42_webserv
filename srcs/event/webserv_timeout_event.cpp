#include "webserv_timeout_event.hpp"

WebservTimeoutEvent::WebservTimeoutEvent()
{
    ;
}

WebservTimeoutEvent::~WebservTimeoutEvent()
{
    ;
}

void dummy_func(WebservTimeoutEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
}

WebservTimeoutEvent *WebservTimeoutEvent::singleton = NULL;
WebservTimeoutEvent *WebservTimeoutEvent::get_instance()
{
    if (WebservTimeoutEvent::singleton == NULL){
        singleton = new WebservTimeoutEvent();
    }
    return (singleton);
}

WebservEvent *WebservTimeoutEvent::make()
{
    WebservTimeoutEvent *timeout_event = WebservTimeoutEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( timeout_event, dummy_func, NULL);
    return (new_event);
}

EWebservEvent WebservTimeoutEvent::which()
{
    return (TIMEOUT_EVENT);
}

WebservEvent* WebservTimeoutEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    WARNING("WebservTimeoutEvent::make_next_event() there is no next event");
    return (NULL);
}

E_EpollEvent WebservTimeoutEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
}

