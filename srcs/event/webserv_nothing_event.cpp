
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservNothingEvent::WebservNothingEvent()
{
}


WebservNothingEvent::~WebservNothingEvent()
{
    ;
}

void dummy_func(WebservNothingEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
}

WebservEvent *WebservNothingEvent::make_nothing_event()
{
    WebservNothingEvent *null_event = new WebservNothingEvent();
    WebservEvent *new_event =  new WebservEvent( null_event, dummy_func, NULL);
    return (new_event);
}

EWebservEvent WebservNothingEvent::which()
{
    return (NOTHING_EVENT);
}

WebservEvent* WebservNothingEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    WARNING("WebservNothingEvent::make_next_event() there is no next event");
    return (NULL);
}

E_EpollEvent WebservNothingEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
}


