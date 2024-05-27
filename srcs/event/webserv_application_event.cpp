#include "webserv_application_event.hpp"
#include "webserv_event.hpp"
#include "method.hpp"

WebservApplicationEvent::WebservApplicationEvent()
{
    ;
}

WebservApplicationEvent::~WebservApplicationEvent()
{
    DEBUG("~WebservApplicationEvent");
}

WebservApplicationEvent *WebservApplicationEvent::singleton = NULL;
WebservApplicationEvent *WebservApplicationEvent::get_instance()
{
    if (WebservApplicationEvent::singleton == NULL){
        singleton = new WebservApplicationEvent();
    }
    return (singleton);
}

void WebservApplicationEvent::delete_myself()
{
    if(WebservApplicationEvent::singleton)
        delete WebservApplicationEvent::singleton;
}


WebservEvent *WebservApplicationEvent::from_event(WebservEvent *event)
{
    WebservApplicationEvent *app_event = WebservApplicationEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( app_event, App::invoke<WebservApplicationEvent>, event->entity());
    return (new_event);
};

WebservEvent* WebservApplicationEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationEvent::make_next_event");
    return (event->entity()->app()->next_event(event, event_factory));
}

E_EpollEvent WebservApplicationEvent::epoll_event(WebservEvent *event)
{
    DEBUG("WebservApplicationEvent::epoll_event");
    if(event->entity()->app() == NULL){
        return (EPOLL_NONE);
    }
    return (event->entity()->app()->epoll_event(event->entity()));
}

void WebservApplicationEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
