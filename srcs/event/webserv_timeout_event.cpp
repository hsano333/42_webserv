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

void dummy_func(WebservTimeoutEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
}

WebservTimeoutEvent *WebservTimeoutEvent::singleton = NULL;
WebservTimeoutEvent *WebservTimeoutEvent::get_instance(FDManager *fd_manager, EventManager *event_manager)
{
    if (WebservTimeoutEvent::singleton == NULL){
        singleton = new WebservTimeoutEvent();
        singleton->fd_manager = fd_manager;
        singleton->event_manager = event_manager;
    }
    return (singleton);
}


void clean_timeout_events(WebservTimeoutEvent *event, WebservEntity *entity)
{
    (void)event;
    std::vector<WebservEvent *> timeout_events;

    event->event_manager->retrieve_timeout_events(timeout_events);
    for(size_t i=0;i<timeout_events.size();i++){
        //force_clean(timeout_events[i], entity->entity()->fd());
        event->close_fd(entity->fd());
    }
    entity->set_completed(true);
}


WebservEvent *WebservTimeoutEvent::make(FDManager *fd_manager, EventManager *event_manager)
{
    WebservTimeoutEvent *timeout_event = WebservTimeoutEvent::get_instance(fd_manager, event_manager);
    WebservEvent *new_event =  new WebservEvent( timeout_event, clean_timeout_events, NULL);
    return (new_event);
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

void WebservTimeoutEvent::close_fd(FileDiscriptor const &fd)
{
    this->fd_manager->close_fd(fd);
}
