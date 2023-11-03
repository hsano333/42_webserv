#include "webserv_write_event.hpp"
#include "socket_writer.hpp"
#include "response.hpp"
#include "error_file.hpp"


WebservWriteEvent::WebservWriteEvent() 
                                        :
                                        fd_(FileDiscriptor::from_int(0)),
                                        req_(NULL),
                                        res_(NULL),
                                        timeout_count_(0),
                                        writer(NULL),
                                        is_completed_(false)
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
        fd_(fd),
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

WebservWriteEvent *WebservWriteEvent::from_error_status_code(WebservEvent *event, StatusCode &code, File *file, IWriter *writer) 
{
    DEBUG("WebservWriteEvent::from_error_status_code()");
    delete event->res();

    Response *res;
    if (file){
        res = Response::from_error_file(file, code);
    }else{
        res = Response::from_error_status_code(code);
    }
    return (new WebservWriteEvent(
            event->fd(),
            event->req(),
            res,
            writer
    ));
}

WebservWriteEvent *WebservWriteEvent::from_event(WebservEvent *event, Response *res, IWriter *writer)
{
    //if(event->res() != NULL){
        //delete event->res();
    //}
    return (new WebservWriteEvent(
            event->fd(),
            event->req(),
            res,
            writer
    ));
}


EWebservEvent WebservWriteEvent::which()
{
    return (WRITE_EVENT);
}


FileDiscriptor WebservWriteEvent::fd()
{
    return (this->fd_);
}

Request *WebservWriteEvent::req()
{
    return (this->req_);
}

Response *WebservWriteEvent::res()
{
    return (this->res_);
}

bool WebservWriteEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservWriteEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
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

WebservEvent* WebservWriteEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservWriteEvent::make_next_event()");
    //return (event_factory->make_application_event(event));
    return (event_factory->make_clean_event(event, false));
}

E_EpollEvent WebservWriteEvent::get_next_epoll_event()
{
    return (EPOLL_NONE);
}


int WebservWriteEvent::write(char const *buf, size_t size)
{

    //char buf[] = "HTTP/1.1 200 OK";
    return (this->writer->write(fd_, buf, size, NULL));
}
