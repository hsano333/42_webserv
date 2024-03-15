
#include "webserv_nothing_event.hpp"
#include "webserv_event.hpp"
#include "global.hpp"

WebservNothingEvent::WebservNothingEvent()
{
}


WebservNothingEvent::~WebservNothingEvent()
{
    ;
}

WebservNothingEvent *WebservNothingEvent::singleton = NULL;
WebservNothingEvent *WebservNothingEvent::get_instance()
{
    if (WebservNothingEvent::singleton == NULL){
        singleton = new WebservNothingEvent();
    }
    return (singleton);
}




WebservEvent *WebservNothingEvent::make_nothing_event()
{
    DEBUG("WebservNothingEvent::make_nothing_event()");
    WebservNothingEvent *null_event = WebservNothingEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( null_event, dummy_func<WebservNothingEvent>, NULL);
    return (new_event);
}

WebservEvent* WebservNothingEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    WARNING("WebservNothingEvent::make_next_event() there is no next event");
    return (NULL);
}

E_EpollEvent WebservNothingEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
}

void WebservNothingEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}

