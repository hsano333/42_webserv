#include "webserv_timeout_event.hpp"

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

void WebservTimeoutEvent::delete_myself()
{
    if(WebservTimeoutEvent::singleton)
        delete WebservTimeoutEvent::singleton;
}

bool none(WebservTimeoutEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
    return (true);
}


bool timeout(WebservTimeoutEvent *event, WebservEntity *entity)
{
    (void)event;
    bool is_close = entity->force_close();
    DEBUG("prepare_clean() + is_close=" + Utility::to_string(is_close));
    if (entity->request()){
        std::string const &conect = entity->request()->header().find(CONNECTION);
        if (conect == "close"){
            is_close = true;
        }
    }

    entity->set_force_close(is_close);
    entity->set_completed(true);
    return (true);
}


WebservEvent *WebservTimeoutEvent::from_event(WebservEvent *event)
{
    WebservTimeoutEvent *timeout_event = WebservTimeoutEvent::get_instance();
    WebservEvent *new_event = new WebservEvent(timeout_event, timeout, event->entity(), KEEP_ALIVE_EVENT);
    return (new_event);
}

WebservEvent *WebservTimeoutEvent::make(WebservEvent *event)
{
    DEBUG("WebservTimeoutEvent::make()");
    event->entity()->set_completed(true);
    WebservTimeoutEvent *timeout_event = WebservTimeoutEvent::get_instance();
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
