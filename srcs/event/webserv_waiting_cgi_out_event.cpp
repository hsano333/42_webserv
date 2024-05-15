#include "webserv_waiting_cgi_out_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>


WebservWaitingCGIOUTEvent::WebservWaitingCGIOUTEvent()
{
    ;
}

WebservWaitingCGIOUTEvent::~WebservWaitingCGIOUTEvent()
{
    ;
}

WebservWaitingCGIOUTEvent *WebservWaitingCGIOUTEvent::singleton = NULL;
WebservWaitingCGIOUTEvent *WebservWaitingCGIOUTEvent::get_instance()
{
    if (WebservWaitingCGIOUTEvent::singleton == NULL){
        singleton = new WebservWaitingCGIOUTEvent();
    }
    return (singleton);
}

WebservEvent *WebservWaitingCGIOUTEvent::from_event(WebservEvent * event)
{
    DEBUG("WebservWaitingCGIOUTEvent::from_fd");
    WebservWaitingCGIOUTEvent *io_event = WebservWaitingCGIOUTEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservWaitingCGIOUTEvent>, event->entity());
    return (new_event);
}

WebservEvent *WebservWaitingCGIOUTEvent::from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    DEBUG("WebservWaitingCGIOUTEvent::from_fd");
    WebservWaitingCGIOUTEvent *io_event = WebservWaitingCGIOUTEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservWaitingCGIOUTEvent>, event->entity());
    new_event->entity()->io().set_read_io(read_src, read_dst);
    new_event->entity()->io().set_write_io(write_src, write_dst);

    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().set_write_fd(write_fd);
    new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}

WebservEvent* WebservWaitingCGIOUTEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservWaitingCGIOUTEvent::make_next_event()");
    //if(event->entity()->io().in_out() == EPOLLIN){
        //DEBUG("WebservWaitingCGIOUTEvent::make_next_event() No.1");
        //return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));

    if(event->entity()->response() == NULL){
        DEBUG("WebservWaitingCGIOUTEvent::make_next_event() No.1");
        //if Response is Null, make it.
        return (event_factory->make_making_response_for_cgi_event(event));
    }
    DEBUG("WebservWaitingCGIOUTEvent::make_next_event() No.2");
    return (event_factory->make_io_socket_for_cgi(event));
}

E_EpollEvent WebservWaitingCGIOUTEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservWaitingCGIOUTEvent::epoll_event()");
    // SCOKET OUT
    if(event->entity()->io().in_out() == EPOLLOUT){
        //end Socket out
        return (EPOLL_FOR_CGI_IN);
    }

    // next event
    if(event->entity()->response() == NULL){
        DEBUG("WebservWaitingCGIOUTEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
        DEBUG("WebservWaitingCGIOUTEvent::epoll_event() No.2");
    return (EPOLL_WRITE);
    /*
    if (event->entity()->completed()){
        DEBUG("WebservWaitingCGIOUTEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI);


    DEBUG("WebservWaitingCGIOUTEvent::epoll_event()");
    //return (EPOLL_FOR_CGI);
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            DEBUG("WebservWaitingCGIOUTEvent::epoll_event() No.1");
            return (EPOLL_NONE);
        }else{
            DEBUG("WebservWaitingCGIOUTEvent::epoll_event() No.2");
            return (EPOLL_READ);
        }
    }
            DEBUG("WebservWaitingCGIOUTEvent::epoll_event() No.3");
    return (EPOLL_NONE);
    */
}

void WebservWaitingCGIOUTEvent::check_completed(WebservEntity * entity)
{
    // EPOLLOUTの時、CGIからの出力
    // EPOLLINの時、SOCKETに対する入力（クライアントに対する出力)
    if(entity->io().in_out() == EPOLLIN){
        entity->set_completed(true);
    }
}
