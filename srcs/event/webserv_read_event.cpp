#include "webserv_read_event.hpp"
#include "socket_reader.hpp"
#include "normal_reader.hpp"
#include "global.hpp"

WebservReadEvent::WebservReadEvent()
                                        :
                                        //req_(new Request()),
                                        req_(NULL),
                                        fd_(FileDiscriptor::from_int(0)),
                                        //event_type(READ_EVENT),
                                        timeout_count_(0),
                                        is_completed_(false)
                                        //ireader(NULL)
{
}

WebservReadEvent::WebservReadEvent(FileDiscriptor fd)
    :
        //req_(new Request()),
        req_(NULL),
        fd_(fd),
        timeout_count_(0)
        //ireader(NULL)
{
    std::cout << "WebservReadEvent Constructor" << std::endl;

    ;
}



WebservReadEvent::~WebservReadEvent()
{
    //delete ireader;
}

WebservReadEvent *WebservReadEvent::from_fd(FileDiscriptor fd)
{
    DEBUG("WebservReadEvent::from_fd() fd:" + Utility::to_string(fd.to_int()));
    WebservReadEvent *event = new WebservReadEvent(fd);
    Request *req = new Request();
    event->req_ = req;
    return (event);

}

WebservReadEvent *WebservReadEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservReadEvent::from_event()");
    WebservReadEvent *new_event = new WebservReadEvent(event->fd());
    DEBUG("WebservReadEvent::from_event() No.1");
    //if (event->req()){
        //delete event->req();
    //}
    if (event->res()){
        DEBUG("WebservReadEvent::from_event() No.11");
        delete event->res();
    }
    DEBUG("WebservReadEvent::from_event() No.2");
    Request *req;
    if(event->req()){
        req = event->req();
    }else{
        req = new Request();
    }
    DEBUG("WebservReadEvent::from_event() No.3");
    new_event->req_ = req;
    DEBUG("WebservReadEvent::from_event() No.4");
    return (new_event);

}

EWebservEvent WebservReadEvent::which()
{
    return (READ_EVENT);
}


FileDiscriptor WebservReadEvent::fd()
{
    return (this->fd_);
}

Request *WebservReadEvent::req()
{
    return (req_);
}

Response *WebservReadEvent::res()
{
    return (NULL);
}

bool WebservReadEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservReadEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}


void WebservReadEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservReadEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservReadEvent::timeout_count()
{
    return (this->timeout_count_);
}


WebservEvent* WebservReadEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservReadEvent::make_next_event()");
    return (event_factory->make_application_event(event));
}

E_EpollEvent WebservReadEvent::get_next_epoll_event()
{
    return (EPOLL_NONE);
}


/*
#include <stdio.h>
int WebservReadEvent::read(char *buf, size_t size)
{

    MYINFO("WebservReadEvent::read() fd:" + Utility::to_string(fd));
    if( this->ireader == NULL){
        std::cout << "buf is NU>>"  << std::endl;
    }
    printf("buf=%p", buf);
    return this->ireader->read(fd, buf, size);
}
*/
