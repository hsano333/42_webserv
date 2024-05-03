#include "webserv_io_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>


WebservIOCGIEvent::WebservIOCGIEvent()
{
    ;
}

WebservIOCGIEvent::~WebservIOCGIEvent()
{
    ;
}

WebservIOCGIEvent *WebservIOCGIEvent::singleton = NULL;
WebservIOCGIEvent *WebservIOCGIEvent::get_instance()
{
    if (WebservIOCGIEvent::singleton == NULL){
        singleton = new WebservIOCGIEvent();
    }
    return (singleton);
}


WebservEvent *WebservIOCGIEvent::from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    WebservIOCGIEvent *io_event = WebservIOCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work<WebservIOCGIEvent>, event->entity());
    new_event->entity()->io().set_read_io(read_src, read_dst);
    new_event->entity()->io().set_write_io(write_src, write_dst);

    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().set_write_fd(write_fd);
    new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}

WebservEvent* WebservIOCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOCGIEvent::make_next_event()");
    if(event->entity()->io().in_out() == EPOLLIN){
        DEBUG("WebservIOCGIEvent::make_next_event() No.1");
        return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));
    }
    return (event);
}

E_EpollEvent WebservIOCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservIOCGIEvent::epoll_event()");

    if (event->entity()->completed()){
        DEBUG("WebservIOCGIEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI);


    DEBUG("WebservIOCGIEvent::epoll_event()");
    //return (EPOLL_FOR_CGI);
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            DEBUG("WebservIOCGIEvent::epoll_event() No.1");
            return (EPOLL_NONE);
        }else{
            DEBUG("WebservIOCGIEvent::epoll_event() No.2");
            return (EPOLL_READ);
        }
    }
            DEBUG("WebservIOCGIEvent::epoll_event() No.3");
    return (EPOLL_NONE);
}

void WebservIOCGIEvent::check_completed(WebservEntity * entity)
{
    DEBUG("WebservIOCGIEvent::check_completed");
    entity->set_completed(true);
}
