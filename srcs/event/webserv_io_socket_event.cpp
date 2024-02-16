#include "webserv_io_socket_event.hpp"
#include "socket_writer.hpp"
#include "opened_socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>


WebservIOSocketEvent::WebservIOSocketEvent()
{
    ;
}

WebservIOSocketEvent::~WebservIOSocketEvent()
{
    ;
}

void dummy_func(WebservIOSocketEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
}

WebservIOSocketEvent *WebservIOSocketEvent::singleton = NULL;
WebservIOSocketEvent *WebservIOSocketEvent::get_instance()
{
    if (WebservIOSocketEvent::singleton == NULL){
        singleton = new WebservIOSocketEvent();
    }
    return (singleton);
}


WebservEvent *WebservIOSocketEvent::as_read(FileDiscriptor const &read_fd, File *src, File *dst, WebservEntity *entity)
{
    WebservIOSocketEvent *io_event = WebservIOSocketEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work<WebservIOSocketEvent>, entity);
    new_event->entity()->io().set_read_io(src, dst);
    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().switching_io(EPOLLIN);

    return (new_event);
}

WebservEvent *WebservIOSocketEvent::as_write(WebservEvent *event, FileDiscriptor const &write_fd, File *src, File *dst)
{
    DEBUG("WebservIOSocketEvent::from_fd fd:" + event->entity()->fd().to_string());
    WebservIOSocketEvent *io_event = WebservIOSocketEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work<WebservIOSocketEvent>, event->entity());
    new_event->entity()->io().set_write_io(src, dst);
    new_event->entity()->io().set_write_fd(write_fd);
    new_event->entity()->io().switching_io(EPOLLOUT);
    return (new_event);
}

WebservEvent* WebservIOSocketEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOSocketEvent::make_next_event()");
    if(event->entity()->io().in_out() == EPOLLIN){
        return (event_factory->make_making_request_event(event));
    }

    return (event_factory->make_clean_event(event, false));
}

E_EpollEvent WebservIOSocketEvent::get_next_epoll_event(WebservEvent *event)
{
    DEBUG("WebservIOSocketEvent::get_next_epoll_event()");
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            return (EPOLL_NONE);
        }else{
            return (EPOLL_READ);
        }
    }
    return (EPOLL_NONE);
}

