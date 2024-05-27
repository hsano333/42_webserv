#include "webserv_waiting_get_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>


WebservWaitingGetCGIEvent::WebservWaitingGetCGIEvent()
{
    ;
}

WebservWaitingGetCGIEvent::~WebservWaitingGetCGIEvent()
{
    ;
}

WebservWaitingGetCGIEvent *WebservWaitingGetCGIEvent::singleton = NULL;
WebservWaitingGetCGIEvent *WebservWaitingGetCGIEvent::get_instance()
{
    if (WebservWaitingGetCGIEvent::singleton == NULL){
        singleton = new WebservWaitingGetCGIEvent();
    }
    return (singleton);
}

void WebservWaitingGetCGIEvent::delete_myself()
{
    if(WebservWaitingGetCGIEvent::singleton)
        delete WebservWaitingGetCGIEvent::singleton;
}

namespace myfunc{
    bool check_completed(WebservWaitingGetCGIEvent *event, WebservEntity *entity)
    {
        event->check_completed(entity);
        return (true);
    }
}

WebservEvent *WebservWaitingGetCGIEvent::from_event(WebservEvent * event)
{
    DEBUG("WebservWaitingGetCGIEvent::from_fd");
    WebservWaitingGetCGIEvent *io_event = WebservWaitingGetCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, myfunc::check_completed, event->entity());
    return (new_event);
}

/*
WebservEvent *WebservWaitingGetCGIEvent::from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    DEBUG("WebservWaitingGetCGIEvent::from_fd");
    WebservWaitingGetCGIEvent *io_event = WebservWaitingGetCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservWaitingGetCGIEvent>, event->entity());
    new_event->entity()->io().set_read_io(read_src, read_dst);
    new_event->entity()->io().set_write_io(write_src, write_dst);

    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().set_write_fd(write_fd);
    new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}
*/

WebservEvent* WebservWaitingGetCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservWaitingGetCGIEvent::make_next_event()");
    //if(event->entity()->io().in_out() == EPOLLIN){
        //DEBUG("WebservWaitingGetCGIEvent::make_next_event() No.1");
        //return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));

    if(event->entity()->response() == NULL){
        DEBUG("WebservWaitingGetCGIEvent::make_next_event() No.1");
        //if Response is Null, make it.
        return (event_factory->make_making_response_for_get_cgi_event(event));
    }
    DEBUG("WebservWaitingGetCGIEvent::make_next_event() No.2");
    return (event_factory->make_io_socket_for_get_cgi(event));
}

E_EpollEvent WebservWaitingGetCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservWaitingGetCGIEvent::epoll_event()");

    // next event
    if(event->entity()->response() == NULL){
        DEBUG("WebservWaitingGetCGIEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
    // SCOKET OUT
    /*
    if(event->entity()->io().in_out() == EPOLLOUT){
        DEBUG("WebservWaitingGetCGIEvent::epoll_event() No.0");
        //end Socket out
        return (EPOLL_FOR_CGI_IN);
    }
    */

    DEBUG("WebservWaitingGetCGIEvent::epoll_event() No.2");
    return (EPOLL_WRITE);
    /*
    if (event->entity()->completed()){
        DEBUG("WebservWaitingGetCGIEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI);


    DEBUG("WebservWaitingGetCGIEvent::epoll_event()");
    //return (EPOLL_FOR_CGI);
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            DEBUG("WebservWaitingGetCGIEvent::epoll_event() No.1");
            return (EPOLL_NONE);
        }else{
            DEBUG("WebservWaitingGetCGIEvent::epoll_event() No.2");
            return (EPOLL_READ);
        }
    }
            DEBUG("WebservWaitingGetCGIEvent::epoll_event() No.3");
    return (EPOLL_NONE);
    */
}

void WebservWaitingGetCGIEvent::check_completed(WebservEntity * entity)
{
        entity->io().set_is_cgi_read(false);
    // EPOLLOUTの時、CGIからの出力
    // EPOLLINの時、SOCKETに対する入力（クライアントに対する出力)
    //if(entity->io().in_out() == EPOLLIN){
        entity->set_completed(true);
    //}
}
