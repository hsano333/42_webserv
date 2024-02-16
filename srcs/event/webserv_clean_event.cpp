
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"
#include "webserv_cleaner.hpp"

WebservCleanEvent::WebservCleanEvent()
{
    ;
}

WebservCleanEvent::~WebservCleanEvent()
{
    ;
}

EWebservEvent WebservCleanEvent::which()
{
    return (CLEAN_EVENT);
}

WebservEvent* WebservCleanEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    MYINFO("WebservCleanEvent::make_next_event");
    if (event->entity()->force_close()){
        MYINFO("WebservCleanEvent::make_next_event() NULL");
        return (NULL);
    }else{
        return (event_factory->make_keep_alive_event(event ));
    }
    (void)event_factory;
    (void)event;
    return (NULL);
}

E_EpollEvent WebservCleanEvent::get_next_epoll_event(WebservEvent *event)
{
    if (event->entity()->force_close()){
        return (EPOLL_CLOSE);
    }else{
        return (EPOLL_READ);
    }
}

WebservCleanEvent *WebservCleanEvent::singleton = NULL;
WebservCleanEvent *WebservCleanEvent::get_instance(FDManager* fd_manager)
{
    if (WebservCleanEvent::singleton == NULL){
        singleton = new WebservCleanEvent();
        singleton->fd_manager = fd_manager;
    }
    return (singleton);
}

WebservEvent *WebservCleanEvent::from_webserv_event(WebservEvent *event, bool force_close, FDManager *fd_manager)
{
    DEBUG("WebservCleanEvent::from_webserv_event");
    WebservCleanEvent *clean_event = WebservCleanEvent::get_instance(fd_manager);
    WebservEvent *new_event =  new WebservEvent(clean_event, clean, event->entity());
    new_event->entity()->set_force_close(force_close);
    new_event->entity()->io().set_source(event->entity()->request());
    return (new_event);
}


void WebservCleanEvent::close_fd(FileDiscriptor const &fd)
{
    this->fd_manager->close_fd(fd);
}
