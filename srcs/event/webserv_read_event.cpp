#include "webserv_read_event.hpp"
#include "socket_reader.hpp"
#include "normal_reader.hpp"
#include "global.hpp"

WebservReadEvent::WebservReadEvent()
                                        :
                                        //req_(new Request()),
                                        req_(NULL),
                                        fd(FileDiscriptor::from_int(0)),
                                        //event_type(READ_EVENT),
                                        timeout_count_(0),
                                        ireader(NULL)
{
}

WebservReadEvent::WebservReadEvent(FileDiscriptor fd)
    :
        req_(new Request()),
        fd(fd),
        timeout_count_(0),
        ireader(NULL)
{
    std::cout << "WebservReadEvent Constructor" << std::endl;

    ;
}

WebservReadEvent::WebservReadEvent(FileDiscriptor fd, IReader *ireader)
    :
        fd(fd),
        timeout_count_(0),
        //event_type(READ_EVENT),
        ireader(ireader)
        //iread()
{
    ;
}


WebservReadEvent::~WebservReadEvent()
{
    //delete ireader;
}

WebservReadEvent *WebservReadEvent::from_fd(FileDiscriptor fd, IReader *reader)
{
    DEBUG("WebservReadEvent::from_fd()");
    WebservReadEvent *event = new WebservReadEvent(fd, reader);
    Request *req = new Request();
    event->req_ = req;
    return (event);

}

EWebservEvent WebservReadEvent::which()
{
    return (READ_EVENT);
}


FileDiscriptor WebservReadEvent::get_fd()
{
    return (this->fd);
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
