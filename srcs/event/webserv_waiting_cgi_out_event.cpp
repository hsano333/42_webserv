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

void WebservWaitingCGIOUTEvent::delete_myself()
{
    if(WebservWaitingCGIOUTEvent::singleton)
        delete WebservWaitingCGIOUTEvent::singleton;
}

WebservEvent *WebservWaitingCGIOUTEvent::from_event(WebservEvent * event)
{
    DEBUG("WebservWaitingCGIOUTEvent::from_fd");
    WebservWaitingCGIOUTEvent *io_event = WebservWaitingCGIOUTEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservWaitingCGIOUTEvent>, event->entity());
    return (new_event);
}


WebservEvent* WebservWaitingCGIOUTEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservWaitingCGIOUTEvent::make_next_event()");

    if(event->entity()->response() == NULL){
        return (event_factory->make_making_response_for_cgi_event(event));
    }
    return (event_factory->make_io_socket_for_cgi(event));
}

E_EpollEvent WebservWaitingCGIOUTEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservWaitingCGIOUTEvent::epoll_event()");

    // SCOKET OUT
    if(event->entity()->io().in_out() == EPOLLOUT){
        return (EPOLL_FOR_CGI_IN);
    }

    // next event
    if(event->entity()->response() == NULL){
        return (EPOLL_NONE);
    }
    return (EPOLL_WRITE);
}

void WebservWaitingCGIOUTEvent::check_completed(WebservEntity * entity)
{
    // EPOLLOUTの時、CGIからの出力
    // EPOLLINの時、SOCKETに対する入力（クライアントに対する出力)
    if(entity->io().in_out() == EPOLLIN){
        entity->set_completed(true);
    }
}
