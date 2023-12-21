#include "webserv_read_event.hpp"
#include "socket_reader.hpp"
#include "opened_socket_file.hpp"
#include "normal_reader.hpp"
#include "global.hpp"

WebservReadEvent::WebservReadEvent()
                                        :
                                        //req_(new Request()),
                                        req_(NULL),
                                        res_(NULL),
                                        source_(NULL),
                                        fd_(FileDiscriptor::from_int(0)),
                                        //event_type(READ_EVENT),
                                        timeout_count_(0),
                                        is_completed_(false),
                                        reader_(NULL)
{
}

/*
WebservReadEvent::WebservReadEvent(FileDiscriptor fd)
    :
        //req_(new Request()),
        req_(NULL),
        fd_(fd),
        timeout_count_(0)
        ireader(NULL)
{
    std::cout << "WebservReadEvent Constructor" << std::endl;

    ;
}
*/

WebservReadEvent::WebservReadEvent(FileDiscriptor fd, IReader *reader)
    :
        //req_(new Request()),
        req_(NULL),
        res_(NULL),
        source_(NULL),
        fd_(fd),
        timeout_count_(0),
        reader_(reader)
{

    std::cout << "WebservReadEvent Constructor" << std::endl;
    //File *file = OpenedSocketFile::from_fd(reader, fd);
    //this->file = file;
}



WebservReadEvent::~WebservReadEvent()
{
    //delete ireader;
}

WebservReadEvent *WebservReadEvent::from_fd(FileDiscriptor fd, FileDiscriptor sockfd, IReader *reader)
{
    DEBUG("WebservReadEvent::from_fd() fd:" + Utility::to_string(fd.to_int()));
    WebservReadEvent *event = new WebservReadEvent(fd, reader);
    DEBUG("WebservReadEvent::from_fd() event:" + Utility::to_string(event));
    //FileDiscriptor sockfd = fd_manager->get_sockfd(event->fd());
    Request *req = Request::from_fd(fd, sockfd);
    File *file = OpenedSocketFile::from_fd(reader, fd);
    req->set_source_file(file);
    //this->file = file;


    event->req_ = req;
    event->source_ = req;
    return (event);
}

WebservReadEvent *WebservReadEvent::from_cgi_fd(FileDiscriptor fd, IReader *reader)
{
    WebservReadEvent *event = new WebservReadEvent(fd, reader);
    return (event);
}

WebservReadEvent *WebservReadEvent::from_event(WebservEvent *event, FileDiscriptor sockfd, IReader *reader)
{
    DEBUG("WebservReadEvent::from_event()");
    WebservReadEvent *new_event = new WebservReadEvent(event->fd(), reader);
    DEBUG("WebservReadEvent::from_event() No.1");
    //if (event->req()){
        //delete event->req();
    //}
    if (event->res()){
        DEBUG("WebservReadEvent::from_event() No.11");
        delete event->res();
    }
    DEBUG("WebservReadEvent::from_event() No.2");
    Request *req;
    if(event->req()){
        req = event->req();
    }else{
        //FileDiscriptor sockfd = fd_manager->get_sockfd(event->fd());
        req = Request::from_fd(event->fd(), sockfd);
        File *file = OpenedSocketFile::from_fd(reader, event->fd());
        req->set_source_file(file);


    }
    DEBUG("WebservReadEvent::from_event() No.3");
    new_event->req_ = req;
    new_event->res_ = NULL;
    new_event->source_ = req;
    DEBUG("WebservReadEvent::from_event() No.4 make event:" + Utility::to_string(new_event));
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

HttpData *WebservReadEvent::source()
{
    return (source_);
}



bool WebservReadEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservReadEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
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


WebservEvent* WebservReadEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservReadEvent::make_next_event()");
    return (event_factory->make_parser_event(event));
    //return (event_factory->make_application_event(event));
}

E_EpollEvent WebservReadEvent::get_next_epoll_event()
{
    if (this->is_completed_){
        return (EPOLL_NONE);
    }else{
        return (EPOLL_READ);
    }
}


#include <stdio.h>
int WebservReadEvent::read(char *buf, size_t size)
{

    MYINFO("WebservReadEvent::read() fd:" + Utility::to_string(this->fd_));
    if( this->reader_ == NULL){
        std::cout << "buf is NU>>"  << std::endl;
    }
    printf("buf=%p", buf);
    return this->reader_->read(this->fd_, buf, size, NULL);
}

void WebservReadEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservReadEvent::cgi_event()
{
    return (this->cgi_event_);
}

/*
IReader *WebservReadEvent::reader()
{
    return (this->reader_);
}
*/



