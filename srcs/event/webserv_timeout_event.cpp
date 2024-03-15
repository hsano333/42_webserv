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
WebservTimeoutEvent *WebservTimeoutEvent::get_instance(WebservCleaner *cleaner)
{
    if (WebservTimeoutEvent::singleton == NULL){
        singleton = new WebservTimeoutEvent();
        singleton->cleaner_ = cleaner;
    }
    return (singleton);
}


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


WebservEvent *WebservTimeoutEvent::make(WebservCleaner *cleaner)
{
    WebservTimeoutEvent *timeout_event = WebservTimeoutEvent::get_instance(cleaner);
    WebservEvent *new_event = new WebservEvent( timeout_event, clean_timeout_events, NULL);
    return (new_event);
}

WebservEvent* WebservTimeoutEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    WARNING("WebservTimeoutEvent::make_next_event() there is no next event");
    return (NULL);
}

E_EpollEvent WebservTimeoutEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
}

WebservCleaner *WebservTimeoutEvent::cleaner() const
{
    return (this->cleaner_);
}

void WebservTimeoutEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
