
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservCleanEvent::WebservCleanEvent()
{
    ;
}

WebservCleanEvent::~WebservCleanEvent()
{
    ;
}

WebservEvent* WebservCleanEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    MYINFO("WebservCleanEvent::make_next_event");
    if (event->entity()->force_close()){
        MYINFO("WebservCleanEvent::make_next_event() >> NULL");
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
WebservCleanEvent *WebservCleanEvent::get_instance(WebservCleaner *cleaner)
{
    if (WebservCleanEvent::singleton == NULL){
        singleton = new WebservCleanEvent();
        singleton->cleaner_ = cleaner;
    }
    return (singleton);
}




/*
WebservEvent *WebservCleanEvent::from_webserv_event(WebservEvent *event, bool force_close, FDManager *fd_manager)
{
    DEBUG("WebservCleanEvent::from_webserv_event");
    WebservCleanEvent *clean_event = WebservCleanEvent::get_instance(fd_manager);
    WebservEvent *new_event =  new WebservEvent(clean_event, clean, event->entity());
    new_event->entity()->set_force_close(force_close);
    new_event->entity()->io().set_source(event->entity()->request());
    return (new_event);
}
*/

bool clean(WebservCleanEvent *event, WebservEntity *entity)
{
    (void)event;

    event->cleaner()->clean(entity, event->force_close());
    return (true);
}

WebservEvent *WebservCleanEvent::from_event(WebservEvent *event, WebservCleaner *cleaner, bool force_close)
{
    DEBUG("WebservCleanEvent::from_webserv_event");
    WebservCleanEvent *clean_event = WebservCleanEvent::get_instance(cleaner);
    clean_event->force_close_ = force_close;
    WebservEvent *new_event = new WebservEvent(clean_event, clean, event->entity());
    //new_event->entity()->set_force_close(force_close);
    new_event->entity()->io().set_source(event->entity()->request());
    return (new_event);
}

bool WebservCleanEvent::force_close()
{
    return (this->force_close_);
}

WebservCleaner *WebservCleanEvent::cleaner() const
{
    return (this->cleaner_);
}
