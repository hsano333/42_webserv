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


WebservEvent *WebservIOCGIEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservIOCGIEvent::from_fd");
    WebservIOCGIEvent *io_event = WebservIOCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work<WebservIOCGIEvent>, event->entity());
    return (new_event);
}

WebservEvent *WebservIOCGIEvent::from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    DEBUG("WebservIOCGIEvent::from_fd");
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
    WebservEvent* new_event;
    if(event->entity()->io().in_out() == EPOLLIN){
        DEBUG("WebservIOCGIEvent::make_next_event() No.1");
        //return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));
        new_event = (event_factory->make_waiting_out_cgi(event));
    }else{

    }
    return (new_event);
}

E_EpollEvent WebservIOCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;


    if (event->entity()->completed()){
        return (EPOLL_NONE);
    }

    DEBUG("WebservIOCGIEvent::epoll_event()");
    if(event->entity()->io().in_out() == EPOLLIN){
        return (EPOLL_WRITE);
    }else{
        return (EPOLL_READ);
    }

    return (EPOLL_NONE);
}

void WebservIOCGIEvent::check_completed(WebservEntity * entity)
{
    //todo 
    entity->set_completed(true);
    return ;
    DEBUG("WebservIOCGIEvent::check_completed");

    // copy from WebservIOSocketEvent
    size_t total_size = entity->io().destination()->size();
    DEBUG("WebservIOCGIEvent::check_completed size=" + Utility::to_string(total_size));

    bool flag = false;
    if(entity->io().in_out() == EPOLLIN){
        // CGI Scriptからの読み取りのみによって決定する。
        DEBUG("WebservIOSocketEvent::check_completed EPOLLIN");
        WebservFile *dst = entity->io().destination_for_read();
        flag = dst->completed();
    }else{ //EPOLL_OUT
        WebservFile *src = entity->io().source_for_write();
        DEBUG("WebservIOSocketEvent::check_completed EPOLLOUT write_size:" + Utility::to_string(src->size()));
        DEBUG("WebservIOSocketEvent::check_completed EPOLLOUT get_content_length():" + Utility::to_string(entity->request()->header().get_content_length()));
        if(src->size() >= (size_t)entity->request()->header().get_content_length()){
            DEBUG("WebservIOSocketEvent::check_completed EPOLLOUT close()");
            src->close();
        }
        //flag = src->completed();
    }

    DEBUG("WebservIOSocketEvent::check_completed end flag:" + Utility::to_string(flag));
    entity->set_completed(flag);
}
