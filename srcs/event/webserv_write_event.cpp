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
                                        //writer(NULL),
                                        is_completed_(false),
                                        entity_(NULL)
{
    ;
}

WebservWriteEvent::WebservWriteEvent(
        //FileDiscriptor fd,
        File *src,
        File *dst
        //File *source
        //IWriter *writer
        )
    :
        //fd_(fd),
        source_file(src),
        destination_file(dst),
        //source_file(source),
        //destination_file(NULL),
        timeout_count_(0)
        //writer(writer)
{
    ;
}

WebservWriteEvent::~WebservWriteEvent()
{
    ;
}

/*
WebservWriteEvent *WebservWriteEvent::from_error_status_code(WebservEvent *event, StatusCode &code, File *file)
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
            res
            //writer
    ));
}

WebservWriteEvent *WebservWriteEvent::from_cgi_fd(FileDiscriptor fd, Request *req, IReader *reader)
{
    DEBUG("WebservWriteEvent::from_cgi_fd()");
    File *file = OpenedSocketFile::from_fd(reader, req->fd());
    //req->set_source_file(file);
    return (new WebservWriteEvent(
            fd,
            req,
            NULL,
            file
            //writer
    ));
}

WebservWriteEvent *WebservWriteEvent::from_event_for_cgi(WebservEvent *event, Response *res)
{
    DEBUG("WebservWriteEvent::from_event_for_cgi()");
    WebservWriteEvent *write_event =  (new WebservWriteEvent(
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

WebservWriteEvent *WebservWriteEvent::from_event(WebservEvent *event, File *src, File *dst)
{
    DEBUG("WebservWriteEvent::from_event()");
    //File *file = OpenedSocketFile::from_fd(writer, res->fd());
    WebservWriteEvent *write_event =  (new WebservWriteEvent(
            //event->fd(),
            src,
            dst
            //writer
    ));
    //write_event->set_cgi_event(event->cgi_event());

    write_event->source_file = src;
    write_event->destination_file = dst;
    //write_event->req_ = event->req();
    //write_event->res_ = event->res();
    write_event->entity_ = event->entity();
    /*
    if(event->res()->get_file()){
            cout << "check file No.1" << endl;
            cout << "check file No.1" << endl;
            cout << "check file No.1" << endl;
    }else{
            cout << "check file not No.1" << endl;
            cout << "check file not No.1" << endl;
            cout << "check file not No.1" << endl;
    }
    */
    //write_event->destination_file = OpenedSocketFile::from_fd(writer, event->fd());

    return (write_event);
}


EWebservEvent WebservWriteEvent::which()
{
    return (WRITE_EVENT);
}


FileDiscriptor &WebservWriteEvent::fd()
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

void WebservWriteEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservWriteEvent::set_dst(File *file)
{
    this->destination_file = file;
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
    DEBUG("WebservWriteEvent::get_next_epoll_event()");
    return (EPOLL_READ);
}


int WebservWriteEvent::write(char *buf, size_t size)
{
    return (this->dst()->write(&buf, size));
}


WebservEntity*WebservWriteEvent::entity()
{
    return (this->entity_);
}

/*
void WebservWriteEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservWriteEvent::cgi_event()
{
    return (this->cgi_event_);
}
*/

