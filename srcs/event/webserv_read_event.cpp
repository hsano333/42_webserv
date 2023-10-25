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
                                        timeout_count_(0)
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
    DEBUG("WebservReadEvent::from_fd()");
    WebservReadEvent *event = new WebservReadEvent(fd);
    Request *req = new Request();
    event->req_ = req;
    return (event);

}

WebservReadEvent *WebservReadEvent::from_event(WebservEvent *event)
{
    WebservReadEvent *new_event = new WebservReadEvent(event->fd());
    //if (event->req()){
        //delete event->req();
    //}
    if (event->res()){
        delete event->res();
    }
    Request *req;
    if(event->req()){
        req = event->req();
    }else{
        req = new Request();
    }
    new_event->req_ = req;
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


void WebservReadEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservReadEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservReadEvent::timeout_count()
{
    return (this->timeout_count_);
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
