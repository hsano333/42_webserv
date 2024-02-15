#include "webserv_timeout_event.hpp"
#include "webserv_cleaner.hpp"

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

WebservEvent *WebservTimeoutEvent::make(FDManager *fd_manager, EventManager *event_manager)
{
    WebservTimeoutEvent *timeout_event = WebservTimeoutEvent::get_instance(fd_manager, event_manager);
    WebservEvent *new_event =  new WebservEvent( timeout_event, clean_timeout_events, NULL);
    return (new_event);
}

EWebservEvent WebservTimeoutEvent::which()
{
    return (TIMEOUT_EVENT);
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
