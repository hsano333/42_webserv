
#include "webserv_keep_alive_event.hpp"
#include "global.hpp"

WebservKeepAliveEvent::WebservKeepAliveEvent()
{
    ;
}

WebservKeepAliveEvent::~WebservKeepAliveEvent()
{
    ;
}

void dummy_func(WebservKeepAliveEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
}

WebservEvent *WebservKeepAliveEvent::from_event(WebservEvent *event)
{
    WebservKeepAliveEvent *keep_event = new WebservKeepAliveEvent();
    WebservEvent *new_event =  new WebservEvent( keep_event, dummy_func, event->entity());
    return (new_event);
}

EWebservEvent WebservKeepAliveEvent::which()
{
    return (KEEPA_ALIVE_EVENT);
}

WebservEvent* WebservKeepAliveEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    WARNING("WebservKeepAliveEvent::make_next_event() there is no next event");
    //
    return (event_factory->make_io_socket_event_as_read(event));
    //return (NULL);
    //return (event);
}

E_EpollEvent WebservKeepAliveEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_READ);
}

