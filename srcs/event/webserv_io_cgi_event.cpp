#include "webserv_io_cgi_event.hpp"
#include "webserv_write_event.hpp"
#include "socket_writer.hpp"
#include "opened_socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>


WebservIOCGIEvent::WebservIOCGIEvent()
{
    ;
}

/*
WebservIOCGIEvent::WebservIOCGIEvent(FileDiscriptor &fd, FileDiscriptor &sock_fd)
                                        :
                                        fd_(fd),
                                        sock_fd_(sock_fd),
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
*/


WebservIOCGIEvent::~WebservIOCGIEvent()
{
    ;
}

void dummy_func(WebservIOCGIEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
    //Request *req = event->make_request(entity);
    //event->entity()->set_request(req);
}

WebservEvent *WebservIOCGIEvent::from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, File *read_src, File *read_dst, File *write_src, File *write_dst, WebservEvent * event)
{
    //DEBUG("WebservIOCGIEvent::from_fd fd:" + fd.to_string());
    WebservIOCGIEvent *io_event = new WebservIOCGIEvent();
    WebservEvent *new_event =  new WebservEvent( io_event, dummy_func, event->entity());
    //event->io = io;
    //event->fd_ = event->fd();
    //event->sock_fd_ = sockfd;
    new_event->entity()->io().set_read_io(read_src, read_dst);
    new_event->entity()->io().set_write_io(write_src, write_dst);
    //event->entity()->io().set_read_dst = read_dst;
    //event->entity()->io().set_write_src = write_src;
    //event->entity()->io().set_write_dst = write_dst;
    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().set_write_fd(write_fd);

    //event->read_fd_ = read_fd;
    //event->write_fd_ = write_fd;

    new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}

/*
WebservIOCGIEvent *WebservIOCGIEvent::from_fd(FileDiscriptor &fd, FileDiscriptor &sockfd, File *io, File *read_src, File *write_dst);
{
    DEBUG("WebservIOCGIEvent::from_fd fd:" + fd.to_string());
    WebservIOCGIEvent *event = new WebservIOCGIEvent(fd, sockfd);
    event->io = dst;
    event->fd_ = event->fd();
    event->sock_fd_ = sockfd;
    event->write_src = src;

    event->switching_io(EPOLLOUT);
    return (event);
}
*/



/*
WebservIOCGIEvent *WebservIOCGIEvent::from_error_status_code(WebservEvent *event, StatusCode &code, File *file)
{
    DEBUG("WebservIOCGIEvent::from_error_status_code()");
    delete event->res();

    Response *res;
    if (file){
        res = Response::from_error_file(file, code);
    }else{
        res = Response::from_error_status_code(code);
    }
    return (new WebservIOCGIEvent(
            event->fd(),
            event->req(),
            res,
            res
            //writer
    ));
}

WebservIOCGIEvent *WebservIOCGIEvent::from_cgi_fd(FileDiscriptor fd, Request *req, IReader *reader)
{
    DEBUG("WebservIOCGIEvent::from_cgi_fd()");
    File *file = OpenedSocketFile::from_fd(reader, req->fd());
    //req->set_source_file(file);
    return (new WebservIOCGIEvent(
            fd,
            req,
            NULL,
            file
            //writer
    ));
}

WebservIOCGIEvent *WebservIOCGIEvent::from_event_for_cgi(WebservEvent *event, Response *res)
{
    DEBUG("WebservIOCGIEvent::from_event_for_cgi()");
    WebservIOCGIEvent *write_event =  (new WebservIOCGIEvent(
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
WebservIOCGIEvent *WebservIOCGIEvent::from_event(WebservEvent *event, File *src, File *dst)
{
    DEBUG("WebservIOCGIEvent::from_event()");
    //File *file = OpenedSocketFile::from_fd(writer, res->fd());
    WebservIOCGIEvent *write_event =  new WebservIOCGIEvent(
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


EWebservEvent WebservIOCGIEvent::which()
{
    return (IO_EVENT);
}


/*
FileDiscriptor &WebservIOCGIEvent::fd()
{
    return (this->fd_);
}

Request *WebservIOCGIEvent::req()
{
    return (this->req_);
}

Response *WebservIOCGIEvent::res()
{
    return (this->res_);
}
*/
/*

File *WebservIOCGIEvent::src()
{
    return (this->source_file);
}

File *WebservIOCGIEvent::dst()
{
    return (this->destination_file);
}


void WebservIOCGIEvent::switching_io(uint32_t epoll_event)
{
    DEBUG("WebservIOCGIEvent::switching_io");
    if(epoll_event == EPOLLIN){
        this->source_file = this->read_src;
        this->destination_file = this->read_dst;
        this->in_out = epoll_event;
    }else if(epoll_event == EPOLLOUT){
        this->source_file = this->write_src;
        this->destination_file = this->write_dst;
        this->in_out = epoll_event;
    }else{
        ERROR("neither EPOLLIN nor EPOLLOUT");
        throw std::runtime_error("neither EPOLLIN nor EPOLLOUT");
    }
}

void WebservIOCGIEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservIOCGIEvent::set_dst(File *file)
{
    this->destination_file = file;
}

void WebservIOCGIEvent::set_write_io(File *src, File *dst)
{
    this->write_src = src;
    this->write_dst = dst;
}

void WebservIOCGIEvent::set_read_io(File *src, File *dst)
{
    this->read_src = src;
    this->read_dst = dst;
}

FileDiscriptor  &WebservIOCGIEvent::get_write_fd()
{
    return (this->write_fd_);
}

FileDiscriptor  &WebservIOCGIEvent::get_read_fd()
{
    return (this->read_fd_);
}
*/

/*
FileDiscriptor  const &WebservIOCGIEvent::get_socket_fd()
{
    return (this->entity()->socket_fd());
}
*/


/*
HttpData *WebservIOCGIEvent::source()
{
    return (this->source_);
}
*/
/*

bool WebservIOCGIEvent::is_completed()
{
    return (this->is_completed_);
}

void WebservIOCGIEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
    //this->is_completed_ = false;
}

void WebservIOCGIEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservIOCGIEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservIOCGIEvent::timeout_count()
{
    return (this->timeout_count_);
}
*/

WebservEvent* WebservIOCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOCGIEvent::make_next_event()");
    if(event->entity()->io().in_out() == EPOLLIN){
        return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));
    }

    DEBUG("WebservIOCGIEvent::make_next_event() EPOLLOUT");
    //EPOLLOUT
    //return (nt_factory->make_clean_event(event, false));
    return (event);
}

E_EpollEvent WebservIOCGIEvent::get_next_epoll_event(WebservEvent *event)
{
    DEBUG("WebservIOCGIEvent::get_next_epoll_event()");
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            return (EPOLL_NONE);
        }else{
            return (EPOLL_READ);
        }
    }

    DEBUG("WebservIOCGIEvent::WRITE ()");
    //EPOLLOUT
    return (EPOLL_NONE);
}

/*
int WebservIOCGIEvent::write(char *buf, size_t size)
{
    return (this->dst()->write(&buf, size));
}

WebservEntity*WebservIOCGIEvent::entity()
{
    return (this->entity_);
}

void WebservIOCGIEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservIOCGIEvent::cgi_event()
{
    return (this->cgi_event_);
}
*/

