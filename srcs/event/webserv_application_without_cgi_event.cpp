#include "webserv_application_without_cgi_event.hpp"
#include "webserv_event.hpp"
#include "method.hpp"

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
    //event->entity->
    DEBUG("WebservApplicationWithoutCgiEvent::make_next_event");
    if(event->entity()->app_result()->method() == POST){
        return (event_factory->make_making_upload_event(event));
    }
    return (event_factory->make_making_response_event(event, event->entity()->io().destination()));
}

E_EpollEvent WebservApplicationWithoutCgiEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    if(event->entity()->app_result()->method() == POST){
        return (EPOLL_READ);
    }
    return (EPOLL_NONE);
}

void WebservApplicationWithoutCgiEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
