
#include "webserv_waiting_post_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>


WebservWaitingPostCGIEvent::WebservWaitingPostCGIEvent()
{
    ;
}

WebservWaitingPostCGIEvent::~WebservWaitingPostCGIEvent()
{
    ;
}

WebservWaitingPostCGIEvent *WebservWaitingPostCGIEvent::singleton = NULL;
WebservWaitingPostCGIEvent *WebservWaitingPostCGIEvent::get_instance()
{
    if (WebservWaitingPostCGIEvent::singleton == NULL){
        singleton = new WebservWaitingPostCGIEvent();
    }
    return (singleton);
}

void WebservWaitingPostCGIEvent::delete_myself()
{
    if(WebservWaitingPostCGIEvent::singleton)
        delete WebservWaitingPostCGIEvent::singleton;
}

namespace myfunc{
    bool check_error(WebservWaitingPostCGIEvent *event, WebservEntity *entity)
    {
        event->check_completed(entity);
        return (true);
    }
}


WebservEvent *WebservWaitingPostCGIEvent::from_event(WebservEvent * event)
{
    DEBUG("WebservWaitingPostCGIEvent::from_event");
    WebservWaitingPostCGIEvent *io_event = WebservWaitingPostCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, myfunc::check_error, event->entity());

    return (new_event);
}

WebservEvent* WebservWaitingPostCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservWaitingPostCGIEvent::make_next_event()");

    if(event->entity()->response() == NULL && event->entity()->io().is_cgi_read() == false){
        DEBUG("WebservWaitingPostCGIEvent::make_next_event() No.1");
        return (event_factory->make_making_response_for_post_cgi_event(event));
    }

    return (event_factory->make_io_socket_for_post_cgi(event));
}

E_EpollEvent WebservWaitingPostCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservWaitingPostCGIEvent::epoll_event()");
    if(event->entity()->response() == NULL && event->entity()->io().is_cgi_read() == false){
        return (EPOLL_NONE);
    }

    if(event->entity()->io().is_cgi_read()){
        DEBUG("WebservWaitingPostCGIEvent::epoll_event() READ");
        WebservFile *file = event->entity()->io().source_for_write();

        if(file->can_read()){
            return (EPOLL_NONE);
        }
        //READ
        return (EPOLL_READ_FOR_POST_CGI);
    }else{
        DEBUG("WebservWaitingPostCGIEvent::epoll_event() WRITE");
        return (EPOLL_WRITE_FOR_POST_CGI);
    }

    return (EPOLL_NONE);
}

void WebservWaitingPostCGIEvent::check_completed(WebservEntity * entity)
{
    DEBUG("WebservWaitingPostCGIEvent::check_completed No.1");
    entity->set_completed(true);

    // EPOLLOUTの時、CGIからの出力
    // EPOLLINの時、SOCKETに対する入力（クライアントに対する出力)
    if(entity->io().in_out() == EPOLLOUT){
        DEBUG("WebservWaitingPostCGIEvent::check_completed No.3");
        entity->io().set_is_cgi_read(true);
    }else{
        entity->io().set_is_cgi_read(false);

    }
}
