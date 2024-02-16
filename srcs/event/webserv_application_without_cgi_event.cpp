#include "webserv_application_without_cgi_event.hpp"
#include "webserv_event.hpp"

WebservApplicationWithoutCgiEvent::WebservApplicationWithoutCgiEvent()
{
    ;
}

WebservApplicationWithoutCgiEvent::~WebservApplicationWithoutCgiEvent()
{
    ;
}


WebservApplicationWithoutCgiEvent *WebservApplicationWithoutCgiEvent::singleton = NULL;
WebservApplicationWithoutCgiEvent *WebservApplicationWithoutCgiEvent::get_instance()
{
    if (WebservApplicationWithoutCgiEvent::singleton == NULL){
        singleton = new WebservApplicationWithoutCgiEvent();
    }
    return (singleton);
}

WebservEvent *WebservApplicationWithoutCgiEvent::from_event(WebservEvent *event)
{
    WebservApplicationWithoutCgiEvent *app_event = WebservApplicationWithoutCgiEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( app_event, invoke<WebservApplicationWithoutCgiEvent>, event->entity());
    return (new_event);
};

WebservEvent* WebservApplicationWithoutCgiEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationWithoutCgiEvent::make_next_event");
    return (event_factory->make_making_response_event(event, event->entity()->io().destination()));
}

E_EpollEvent WebservApplicationWithoutCgiEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
}

