
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "webserv_file_factory.hpp"
#include "header_word.hpp"
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
    MYINFO("WebservCleanEvent::make_next_event fd:" + event->entity()->fd().to_string());
    if (event->entity()->force_close()){
        MYINFO("WebservCleanEvent::make_next_event() >> NULL");
        return (NULL);
    }else{
        return (event_factory->make_keep_alive_event(event ));
    }
    return (NULL);
}

E_EpollEvent WebservCleanEvent::epoll_event(WebservEvent *event)
{
    return (EPOLL_NONE);
    if (event->entity()->force_close()){
        return (EPOLL_CLOSE);
    }else{
        return (EPOLL_READ);
    }
}

WebservCleanEvent *WebservCleanEvent::singleton = NULL;
WebservCleanEvent *WebservCleanEvent::get_instance()
{
    if (WebservCleanEvent::singleton == NULL){
        singleton = new WebservCleanEvent();
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

/*
bool clean(WebservCleanEvent *event, WebservEntity *entity)
{
    (void)event;

    event->cleaner()->clean(entity, event->force_close());
    return (true);
}
*/



bool prepare_clean(WebservCleanEvent *event, WebservEntity *entity)
{
    (void)event;
    bool is_close = entity->force_close();
    //bool is_close = event->force_close() || entity->force_close();
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

WebservEvent *WebservCleanEvent::from_event(WebservEvent *event, bool force_close)
{
    DEBUG("WebservCleanEvent::from_webserv_event");
    WebservCleanEvent *clean_event = WebservCleanEvent::get_instance();
    WebservEvent *new_event = new WebservEvent(clean_event, prepare_clean, event->entity(), CLEAN_EVENT);
    event->entity()->set_force_close(force_close || event->entity()->force_close());

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *file = file_factory->make_webserv_file_regular(event->entity()->fd(), event->entity()->request());

    new_event->entity()->io().set_source(file);
    return (new_event);
}

void WebservCleanEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
