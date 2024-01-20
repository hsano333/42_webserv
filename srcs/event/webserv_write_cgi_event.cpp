#include "webserv_write_cgi_event.hpp"
#include "socket_writer.hpp"
#include "opened_socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"


WebservWriteCGIEvent::WebservWriteCGIEvent()
                                        :
                                        fd_(FileDiscriptor::from_int(0)),
                                        req_(NULL),
                                        res_(NULL),
                                        source_file(NULL),
                                        destination_file(NULL),
                                        timeout_count_(0),
                                        //writer(NULL),
                                        is_completed_(false)
{
    ;
}

WebservWriteCGIEvent::WebservWriteCGIEvent(FileDiscriptor fd)
                                        :
                                        fd_(fd),
                                        req_(NULL),
                                        res_(NULL),
                                        source_file(NULL),
                                        destination_file(NULL),
                                        timeout_count_(0),
                                        //writer(NULL),
                                        is_completed_(false)
{
    ;
}


WebservWriteCGIEvent::~WebservWriteCGIEvent()
{
    ;
}

/*
WebservWriteCGIEvent *WebservWriteCGIEvent::from_error_status_code(WebservEvent *event, StatusCode &code, File *file, IWriter *writer)
{
    DEBUG("WebservWriteCGIEvent::from_error_status_code()");
    delete event->res();

    Response *res;
    if (file){
        res = Response::from_error_file(file, code);
    }else{
        res = Response::from_error_status_code(code);
    }
    return (new WebservWriteCGIEvent(
            event->fd(),
            event->req(),
            res,
            res,
            writer
    ));
}

WebservWriteCGIEvent *WebservWriteCGIEvent::from_cgi_fd(FileDiscriptor fd, Request *req, IReader *reader, IWriter *writer)
{
    DEBUG("WebservWriteCGIEvent::from_cgi_fd()");
    File *file = OpenedSocketFile::from_fd(reader, req->fd());
    //req->set_source_file(file);
    return (new WebservWriteCGIEvent(
            fd,
            req,
            NULL,
            file,
            writer
    ));
}
*/

/*
WebservWriteCGIEvent *WebservWriteCGIEvent::from_event_for_cgi(WebservEvent *event, Response *res, IWriter *writer)
{
    DEBUG("WebservWriteCGIEvent::from_event_for_cgi()");
    WebservWriteCGIEvent *write_event =  (new WebservWriteCGIEvent(
            event->fd(),
            event->req(),
            res,
            NULL,
            writer
    ));
    //write_event->set_cgi_event(event->cgi_event());
    return (write_event);
}
*/

WebservWriteCGIEvent *WebservWriteCGIEvent::from_event(WebservEvent *event, File *src, File *dst, ApplicationResult *result)
{
    DEBUG("WebservWriteCGIEvent::from_event()");
    //File *file = OpenedSocketFile::from_fd(writer, res->fd());
    WebservWriteCGIEvent *write_event =  new WebservWriteCGIEvent(event->fd());
    write_event->req_ = event->req();
    write_event->res_ = event->res();
    write_event->source_file = src;
    write_event->destination_file = dst;
    write_event->result = result;
    //write_event->set_cgi_event(event->cgi_event());
    return (write_event);
}

EWebservEvent WebservWriteCGIEvent::which()
{
    return (WRITE_EVENT);
}

FileDiscriptor WebservWriteCGIEvent::fd()
{
    return (this->fd_);
}

Request *WebservWriteCGIEvent::req()
{
    return (this->req_);
}

Response *WebservWriteCGIEvent::res()
{
    return (this->res_);
}

File *WebservWriteCGIEvent::src()
{
    return (this->source_file);
}

File *WebservWriteCGIEvent::dst()
{
    return (this->destination_file);
}


void WebservWriteCGIEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservWriteCGIEvent::set_dst(File *file)
{
    this->destination_file = file;
}

/*
HttpData *WebservWriteCGIEvent::source()
{
    return (this->source_);
}
*/

bool WebservWriteCGIEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservWriteCGIEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}


void WebservWriteCGIEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservWriteCGIEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservWriteCGIEvent::timeout_count()
{
    return (this->timeout_count_);
}

WebservEvent* WebservWriteCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event;
    (void)event_factory;

    //WebservCgiEvent *cgi = event->cgi_event();
    DEBUG("WebservWriteCGIEvent::make_next_event()");
    //return (event_factory->make_nothing_event(cgi->fd_in(), cgi->pid()));
    //todo
    return (NULL);
}

E_EpollEvent WebservWriteCGIEvent::get_next_epoll_event()
{
    return (EPOLL_CGI_STOP);
}


int WebservWriteCGIEvent::write(char *buf, size_t size)
{
    return (this->dst()->write(&buf, size));
}

/*
void WebservWriteCGIEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}
*/

/*
WebservCgiEvent *WebservWriteCGIEvent::cgi_event()
{
    return (this->cgi_event_);
}
*/

