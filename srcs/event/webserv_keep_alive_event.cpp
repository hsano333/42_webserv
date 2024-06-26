
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


WebservKeepAliveEvent *WebservKeepAliveEvent::singleton = NULL;
WebservKeepAliveEvent *WebservKeepAliveEvent::get_instance()
{
    if (WebservKeepAliveEvent::singleton == NULL){
        singleton = new WebservKeepAliveEvent();
    }
    return (singleton);
}

void WebservKeepAliveEvent::delete_myself()
{
    if(WebservKeepAliveEvent::singleton)
        delete WebservKeepAliveEvent::singleton;
}

WebservEvent *WebservKeepAliveEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservKeepAliveEvent::from_event");
    WebservKeepAliveEvent *keep_event = WebservKeepAliveEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( keep_event, dummy_func<WebservKeepAliveEvent>, event->entity(), KEEP_ALIVE_EVENT);
    return (new_event);
}

WebservEvent* WebservKeepAliveEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservKeepAliveEvent::make_next_event");
    (void)event_factory;
    (void)event;
    return (event_factory->make_io_socket_event_as_read(event));
}

E_EpollEvent WebservKeepAliveEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_READ);
}

void WebservKeepAliveEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
