#include "webserv_read_event.hpp"
#include "read_socket.hpp"
#include "read_raw.hpp"
#include "global.hpp"

WebservReadEvent::WebservReadEvent()
                                        :
                                        //req_(new Request()),
                                        req_(NULL),
                                        fd(FileDiscriptor::from_int(0)),
                                        //event_type(READ_EVENT),
                                        timeout_count(0),
                                        ireader(new ReadSocket())
{
}

WebservReadEvent::WebservReadEvent(FileDiscriptor fd)
    :
        req_(new Request()),
        fd(fd),
        timeout_count(0),
        ireader(new ReadSocket())
{
    std::cout << "WebservReadEvent Constructor" << std::endl;

    ;
}

WebservReadEvent::WebservReadEvent(FileDiscriptor fd, IReader *ireader)
    :
        fd(fd),
        timeout_count(0),
        //event_type(READ_EVENT),
        ireader(ireader)
        //iread()
{
    ;
}


WebservReadEvent::~WebservReadEvent()
{
    delete ireader;
}

WebservReadEvent *WebservReadEvent::from_fd(FileDiscriptor fd)
{
    DEBUG("WebservReadEvent::from_fd()");
    WebservReadEvent *event = new WebservReadEvent(fd);
    //WebservReadEvent *event = new WebservReadEvent();
    DEBUG("WebservReadEvent::from_fd() 2");
    //event->fd = fd;
    DEBUG("WebservReadEvent::from_fd() 3");
    Request *req = new Request();
    DEBUG("WebservReadEvent::from_fd() 4");
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


void WebservReadEvent::increase_and_check_timeout_count(int count)
{
    this->timeout_count += count;
    if(this->timeout_count >= TIMEOUT){
        DEBUG("Exceed Timeout:" + Utility::to_string(TIMEOUT) + "sec");
    }
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
