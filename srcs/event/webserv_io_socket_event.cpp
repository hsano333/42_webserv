#include "webserv_io_socket_event.hpp"
#include "webserv_write_event.hpp"
#include "socket_writer.hpp"
#include "opened_socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"


WebservIOSocketEvent::WebservIOSocketEvent()
                                        :
                                        fd_(FileDiscriptor::from_int(0)),
                                        sock_fd_(FileDiscriptor::from_int(0)),
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


WebservIOSocketEvent::~WebservIOSocketEvent()
{
    ;
}


WebservIOSocketEvent *WebservIOSocketEvent::from_fd(FileDiscriptor &fd, FileDiscriptor &sockfd, File *socket_io, File *read_dst)
{
    DEBUG("WebservIOSocketEvent::from_fd fd:" + fd.to_string());
    WebservIOSocketEvent *event = new WebservIOSocketEvent();
    event->io = socket_io;
    event->fd_ = sockfd;
    event->sock_fd_ = sockfd;
    event->read_dst = read_dst;

    return (event);
}

/*
WebservIOSocketEvent *WebservIOSocketEvent::from_error_status_code(WebservEvent *event, StatusCode &code, File *file)
{
    DEBUG("WebservIOSocketEvent::from_error_status_code()");
    delete event->res();

    Response *res;
    if (file){
        res = Response::from_error_file(file, code);
    }else{
        res = Response::from_error_status_code(code);
    }
    return (new WebservIOSocketEvent(
            event->fd(),
            event->req(),
            res,
            res
            //writer
    ));
}

WebservIOSocketEvent *WebservIOSocketEvent::from_cgi_fd(FileDiscriptor fd, Request *req, IReader *reader)
{
    DEBUG("WebservIOSocketEvent::from_cgi_fd()");
    File *file = OpenedSocketFile::from_fd(reader, req->fd());
    //req->set_source_file(file);
    return (new WebservIOSocketEvent(
            fd,
            req,
            NULL,
            file
            //writer
    ));
}

WebservIOSocketEvent *WebservIOSocketEvent::from_event_for_cgi(WebservEvent *event, Response *res)
{
    DEBUG("WebservIOSocketEvent::from_event_for_cgi()");
    WebservIOSocketEvent *write_event =  (new WebservIOSocketEvent(
            event->fd(),
            event->req(),
            res,
            NULL
            //writer
    ));
    write_event->set_cgi_event(event->cgi_event());
    return (write_event);
}
*/

/*
WebservIOSocketEvent *WebservIOSocketEvent::from_event(WebservEvent *event, File *src, File *dst)
{
    DEBUG("WebservIOSocketEvent::from_event()");
    //File *file = OpenedSocketFile::from_fd(writer, res->fd());
    WebservIOSocketEvent *write_event =  new WebservIOSocketEvent(
            event->fd(),
            src,
            dst
            //writer
    );
    //write_event->set_cgi_event(event->cgi_event());

    write_event->source_file = src;
    write_event->destination_file = dst;
    write_event->req_ = event->req();
    write_event->res_ = event->res();
    if(event->res()->get_file()){
            cout << "check file No.1" << endl;
            cout << "check file No.1" << endl;
            cout << "check file No.1" << endl;
    }else{
            cout << "check file not No.1" << endl;
            cout << "check file not No.1" << endl;
            cout << "check file not No.1" << endl;
    }
    //write_event->destination_file = OpenedSocketFile::from_fd(writer, event->fd());

    return (write_event);
}
*/


EWebservEvent WebservIOSocketEvent::which()
{
    return (IO_EVENT);
}


FileDiscriptor WebservIOSocketEvent::fd()
{
    return (this->fd_);
}

Request *WebservIOSocketEvent::req()
{
    return (this->req_);
}

Response *WebservIOSocketEvent::res()
{
    return (this->res_);
}

File *WebservIOSocketEvent::src()
{
    return (this->source_file);
}

File *WebservIOSocketEvent::dst()
{
    return (this->destination_file);
}


void WebservIOSocketEvent::set_io(uint32_t epoll_event)
{
    if(epoll_event == EPOLLIN){
        this->source_file = this->io;
        this->destination_file = this->read_dst;
    }else{
        this->source_file = this->write_src;
        this->destination_file = this->io;
    }
}

void WebservIOSocketEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservIOSocketEvent::set_dst(File *file)
{
    this->destination_file = file;
}

/*
HttpData *WebservIOSocketEvent::source()
{
    return (this->source_);
}
*/

bool WebservIOSocketEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservIOSocketEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}


void WebservIOSocketEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservIOSocketEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservIOSocketEvent::timeout_count()
{
    return (this->timeout_count_);
}

WebservEvent* WebservIOSocketEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOSocketEvent::make_next_event()");
    //return (event_factory->make_application_event(event));
    return (event_factory->make_clean_event(event, false));
}

E_EpollEvent WebservIOSocketEvent::get_next_epoll_event()
{
    DEBUG("WebservIOSocketEvent::get_next_epoll_event()");
    return (EPOLL_READ);
}


int WebservIOSocketEvent::write(char *buf, size_t size)
{
    return (this->dst()->write(&buf, size));
}

/*
void WebservIOSocketEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservIOSocketEvent::cgi_event()
{
    return (this->cgi_event_);
}
*/

