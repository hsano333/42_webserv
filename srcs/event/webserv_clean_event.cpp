
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
    DEBUG("WebservCleanEvent::make_next_event fd:" + event->entity()->fd().to_string());
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
    if (event->entity()->force_close()){
        return (EPOLL_CLOSE);
    }else{
        // keep alive
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

void WebservCleanEvent::delete_myself()
{
    if(WebservCleanEvent::singleton)
        delete WebservCleanEvent::singleton;
}

bool prepare_clean(WebservCleanEvent *event, WebservEntity *entity)
{
    (void)event;
    bool is_close = entity->force_close();
    DEBUG("prepare_clean() + is_close=" + Utility::to_string(is_close));
    entity->set_completed(true);
    if(entity == NULL){
        return true;
    }
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
    DEBUG("WebservCleanEvent::from_webserv_event force_close=" + Utility::to_string(force_close));
    WebservCleanEvent *clean_event = WebservCleanEvent::get_instance();
    WebservEvent *new_event = new WebservEvent(clean_event, prepare_clean, event->entity(), CLEAN_EVENT);
    event->entity()->set_force_close(force_close || event->entity()->force_close());

    return (new_event);
}

void WebservCleanEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
