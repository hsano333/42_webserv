#include "webserv_io_socket_event.hpp"
#include "http_exception.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
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


WebservIOSocketEvent *WebservIOSocketEvent::singleton = NULL;
WebservIOSocketEvent *WebservIOSocketEvent::get_instance()
{
    if (WebservIOSocketEvent::singleton == NULL){
        singleton = new WebservIOSocketEvent();
    }
    return (singleton);
}

void WebservIOSocketEvent::delete_myself()
{
    if(WebservIOSocketEvent::singleton)
        delete WebservIOSocketEvent::singleton;
}


WebservEvent *WebservIOSocketEvent::as_read(FileDiscriptor const &read_fd, WebservFile *src, WebservFile *dst, WebservEntity *entity)
{
    (void)read_fd;
    DEBUG("WebservIOSocketEvent::as_read fd:" + read_fd.to_string());
    WebservIOSocketEvent *io_event = WebservIOSocketEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work<WebservIOSocketEvent>, entity);
    new_event->entity()->io().set_read_io(src, dst);
    new_event->entity()->io().switching_io(EPOLLIN);
    new_event->entity()->io().set_total_write_size(0);

    return (new_event);
}

WebservEvent *WebservIOSocketEvent::as_write(WebservEvent *event, FileDiscriptor const &write_fd, WebservFile *src, WebservFile *dst)
{
    DEBUG("WebservIOSocketEvent::as_write fd:" + event->entity()->fd().to_string());
    WebservIOSocketEvent *io_event = WebservIOSocketEvent::get_instance();
    WebservEvent *new_event;
    new_event = new WebservEvent( io_event, io_work_ref<WebservIOSocketEvent>, event->entity());
    new_event->entity()->io().set_write_io(src, dst);
    (void)write_fd;
    new_event->entity()->io().switching_io(EPOLLOUT);
    return (new_event);
}

WebservEvent *WebservIOSocketEvent::as_chunked_write(WebservEvent *event, FileDiscriptor const &write_fd, WebservFile *src, WebservFile *dst)
{
    (void)write_fd;
    DEBUG("WebservIOSocketEvent::from_fd fd:" + event->entity()->fd().to_string());
    WebservIOSocketEvent *io_event = WebservIOSocketEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work_ref<WebservIOSocketEvent>, event->entity());
    new_event->entity()->io().set_write_io(src, dst);
    new_event->entity()->io().switching_io(EPOLLOUT);
    return (new_event);
}

WebservEvent* WebservIOSocketEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    if(event->entity()->io().in_out() == EPOLLIN){
        DEBUG("WebservIOSocketEvent::make_next_event() EPOLLIN");
        return (event_factory->make_making_request_event(event));
    }
    DEBUG("WebservIOSocketEvent::make_next_event() EPOLLOUT");
    return (event_factory->make_clean_event(event, false));
}

E_EpollEvent WebservIOSocketEvent::epoll_event(WebservEvent *event)
{
    DEBUG("WebservIOSocketEvent::epoll_event()");
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            return (EPOLL_NONE);
        }else{
            return (EPOLL_READ);
        }
    }else{ 
        //EPOLLOUT
        if (event->entity()->completed()){
            return (EPOLL_NONE);
        }else{
            return (EPOLL_WRITE);
        }

    }
    return (EPOLL_NONE);
}

void WebservIOSocketEvent::check_completed(WebservEntity * entity)
{
    bool flag = false;
    if(entity->io().in_out() == EPOLLIN){
        DEBUG("WebservIOSocketEvent::check_completed EPOLLIN");
        WebservFile *dst = entity->io().destination();
        flag = dst->completed();
    }else{
        DEBUG("WebservIOSocketEvent::check_completed EPOLLOUT");
        Response *res= entity->response();
        flag = res->read_completed();
    }

    entity->set_completed(flag);
}
