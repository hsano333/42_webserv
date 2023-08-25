#include "webserv_event.hpp"

WebservEvent::WebservEvent()
{
    ;
}

WebservEvent::~WebservEvent()
{
    ;
}

EWebservEvent WebservEvent::which()
{
    return (READ_EVENT);
}


FileDiscriptor WebservEvent::get_fd()
{
    return (this->fd);
}


WebservEvent *WebservEvent::from_epoll_event(t_epoll_event const &event_epoll)
{
    WebservEvent *event = new WebservEvent();
    event->fd = FileDiscriptor::from_int(event_epoll.data.fd);
    if(event_epoll.events == EPOLLIN){
        event->event_type = READ_EVENT;
    }else if(event_epoll.events == EPOLLOUT){
        event->event_type = WRITE_EVENT;
    }else{
        WARNING("Epoll event type is undefined");
        throw std::runtime_error("Epoll event type is undefined");

    }
    return (event);
}

