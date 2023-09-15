#include "webserv_write_event.hpp"
#include "socket_writer.hpp"


WebservWriteEvent::WebservWriteEvent() 
                                        :
                                        fd(FileDiscriptor::from_int(0)),
                                        req_(NULL),
                                        res_(NULL),
                                        timeout_count_(0),
                                        writer(NULL)
{
    ;
}

WebservWriteEvent::WebservWriteEvent(
        FileDiscriptor fd,
        Request *req,
        Response *res,
        IWriter *writer
        )
    :
        fd(fd),
        req_(req),
        res_(res),
        timeout_count_(0),
        writer(writer)
{
    ;
}

WebservWriteEvent::~WebservWriteEvent()
{
    ;
}

WebservWriteEvent *WebservWriteEvent::from_event(WebservEvent *event, Response *res, IWriter *writer)
{
    //if(event->res() != NULL){
        //delete event->res();
    //}
    return (new WebservWriteEvent(
            event->get_fd(),
            event->req(),
            res,
            writer
    ));
}


EWebservEvent WebservWriteEvent::which()
{
    return (WRITE_EVENT);
}


FileDiscriptor WebservWriteEvent::get_fd()
{
    return (this->fd);
}

Request *WebservWriteEvent::req()
{
    return (this->req_);
}

Response *WebservWriteEvent::res()
{
    return (this->res_);
}


void WebservWriteEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservWriteEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservWriteEvent::timeout_count()
{
    return (this->timeout_count_);
}

int WebservWriteEvent::write(char const *buf, size_t size)
{

    //char buf[] = "HTTP/1.1 200 OK";
    return (this->writer->write(fd, buf, size));
}
