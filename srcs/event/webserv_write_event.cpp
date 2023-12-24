#include "webserv_write_event.hpp"
#include "socket_writer.hpp"
#include "opened_socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"


WebservWriteEvent::WebservWriteEvent()
                                        :
                                        fd_(FileDiscriptor::from_int(0)),
                                        req_(NULL),
                                        res_(NULL),
                                        source_file(NULL),
                                        destination_file(NULL),
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
        File *source,
        IWriter *writer
        )
    :
        fd_(fd),
        req_(req),
        res_(res),
        source_file(source),
        destination_file(NULL),
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
            file,
            writer
    ));
}

WebservWriteEvent *WebservWriteEvent::from_cgi_fd(FileDiscriptor fd, Request *req, IReader *reader, IWriter *writer)
{
    DEBUG("WebservWriteEvent::from_cgi_fd()");
    File *file = OpenedSocketFile::from_fd(reader, req->fd());
    //req->set_source_file(file);
    return (new WebservWriteEvent(
            fd,
            req,
            NULL,
            file,
            writer
    ));
}

WebservWriteEvent *WebservWriteEvent::from_event_for_cgi(WebservEvent *event, Response *res, IWriter *writer)
{
    DEBUG("WebservWriteEvent::from_event_for_cgi()");
    WebservWriteEvent *write_event =  (new WebservWriteEvent(
            event->fd(),
            event->req(),
            res,
            NULL,
            writer
    ));
    write_event->set_cgi_event(event->cgi_event());
    return (write_event);
}

WebservWriteEvent *WebservWriteEvent::from_event(WebservEvent *event, Response *res, IWriter *writer)
{
    DEBUG("WebservWriteEvent::from_event()");
    //File *file = OpenedSocketFile::from_fd(writer, res->fd());
    WebservWriteEvent *write_event =  (new WebservWriteEvent(
            event->fd(),
            event->req(),
            res,
            res,
            writer
    ));
    write_event->set_cgi_event(event->cgi_event());
    return (write_event);
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

File *WebservWriteEvent::src()
{
    return (this->source_file);
}

File *WebservWriteEvent::dst()
{
    return (this->destination_file);
}

/*
HttpData *WebservWriteEvent::source()
{
    return (this->source_);
}
*/

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
    return (EPOLL_READ);
}


int WebservWriteEvent::write(char const *buf, size_t size)
{
    return (this->writer->write(fd_, buf, size, NULL));
}

void WebservWriteEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservWriteEvent::cgi_event()
{
    return (this->cgi_event_);
}

