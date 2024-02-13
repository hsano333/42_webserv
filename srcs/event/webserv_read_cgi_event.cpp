#include "webserv_read_cgi_event.hpp"
#include "socket_reader.hpp"
#include "opened_socket_file.hpp"
#include "normal_reader.hpp"
#include "global.hpp"

WebservReadCGIEvent::WebservReadCGIEvent()
                                        :
                                        req_(NULL),
                                        res_(NULL),
                                        source_file(NULL),
                                        destination_file(NULL),
                                        fd_(FileDiscriptor::from_int(0)),
                                        //event_type(READ_EVENT),
                                        timeout_count_(0),
                                        is_completed_(false),
                                        reader(NULL)
{
}

/*
WebservReadCGIEvent::WebservReadCGIEvent(FileDiscriptor fd)
    :
        //req_(new Request()),
        req_(NULL),
        fd_(fd),
        timeout_count_(0)
        ireader(NULL)
{
    std::cout << "WebservReadCGIEvent Constructor" << std::endl;

    ;
}
*/

WebservReadCGIEvent::WebservReadCGIEvent(FileDiscriptor fd, IReader *reader)
    :
        //req_(new Request()),
        req_(NULL),
        res_(NULL),
        source_file(NULL),
        destination_file(NULL),
        fd_(fd),
        timeout_count_(0),
        reader(reader)
{

    std::cout << "WebservReadCGIEvent Constructor" << std::endl;
    //File *file = OpenedSocketFile::from_fd(reader, fd);
    //this->file = file;
}



WebservReadCGIEvent::~WebservReadCGIEvent()
{
    //delete ireader;
}

WebservReadCGIEvent *WebservReadCGIEvent::from_fd(FileDiscriptor fd, FileDiscriptor sockfd, IReader *reader)
{
    DEBUG("WebservReadCGIEvent::from_fd() fd:" + Utility::to_string(fd.to_int()));
    WebservReadCGIEvent *event = new WebservReadCGIEvent(fd, reader);
    DEBUG("WebservReadCGIEvent::from_fd() event:" + Utility::to_string(event));
    //FileDiscriptor sockfd = fd_manager->get_sockfd(event->fd());
    Request *req = Request::from_fd(fd, sockfd);
    event->source_file = OpenedSocketFile::from_fd(reader, fd);
    //File *file = OpenedSocketFile::from_fd(reader, fd);
    //req->set_source_file(file);
    //this->file = file;


    event->req_ = req;
    //event->source_file = req;
    return (event);
}

WebservReadCGIEvent *WebservReadCGIEvent::from_cgi_fd(FileDiscriptor fd, IReader *reader)
{
    WebservReadCGIEvent *event = new WebservReadCGIEvent(fd, reader);
    event->source_file = OpenedSocketFile::from_fd(reader, fd);
    //req->set_source_file(file);
    return (event);
}

WebservReadCGIEvent *WebservReadCGIEvent::from_event(WebservEvent *event, FileDiscriptor sockfd, IReader *reader)
{
    DEBUG("WebservReadCGIEvent::from_event()");
    WebservReadCGIEvent *new_event = new WebservReadCGIEvent(event->fd(), reader);
    DEBUG("WebservReadCGIEvent::from_event() No.1");
    //if (event->req()){
        //delete event->req();
    //}
    if (event->res()){
        DEBUG("WebservReadCGIEvent::from_event() No.11");
        delete event->res();
    }
    DEBUG("WebservReadCGIEvent::from_event() No.2");
    Request *req;
    if(event->req()){
        req = event->req();
    }else{
        //FileDiscriptor sockfd = fd_manager->get_sockfd(event->fd());
        req = Request::from_fd(event->fd(), sockfd);
        //File *file = OpenedSocketFile::from_fd(reader, event->fd());
        //new_event->source_file = OpenedSocketFile::from_fd(reader, event->fd());
        //req->set_source_file(file);
    }
    DEBUG("WebservReadCGIEvent::from_event() No.3");
    new_event->req_ = req;
    new_event->res_ = NULL;
    new_event->source_file = req;
    DEBUG("WebservReadCGIEvent::from_event() No.4 make event:" + Utility::to_string(new_event));
    return (new_event);

}

EWebservEvent WebservReadCGIEvent::which()
{
    return (READ_EVENT);
}


FileDiscriptor &WebservReadCGIEvent::fd()
{
    return (this->fd_);
}

Request *WebservReadCGIEvent::req()
{
    return (req_);
}

Response *WebservReadCGIEvent::res()
{
    return (NULL);
}

File *WebservReadCGIEvent::src()
{
    return (this->source_file);
}

File *WebservReadCGIEvent::dst()
{
    return (this->destination_file);
}

bool WebservReadCGIEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservReadCGIEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}


void WebservReadCGIEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservReadCGIEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservReadCGIEvent::timeout_count()
{
    return (this->timeout_count_);
}


WebservEvent* WebservReadCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservReadCGIEvent::make_next_event()");
    return (event_factory->make_parser_event(event));
    //return (event_factory->make_application_event(event));
}

E_EpollEvent WebservReadCGIEvent::get_next_epoll_event(WebservEvent *event)
{
    if (event->is_completed_){
        return (EPOLL_NONE);
    }else{
        return (EPOLL_READ);
    }
}


#include <stdio.h>
int WebservReadCGIEvent::read(char *buf, size_t size)
{

    MYINFO("WebservReadCGIEvent::read() fd:" + Utility::to_string(this->fd_));
    if( this->reader == NULL){
        std::cout << "buf is NU>>"  << std::endl;
    }
    printf("buf=%p", buf);
    return this->reader->read(this->fd_, buf, size, NULL);
}

void WebservReadCGIEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservReadCGIEvent::cgi_event()
{
    return (this->cgi_event_);
}

WebservEntity*WebservReadCGIEvent::entity()
{
    return (this->entity_);
}
/*
IReader *WebservReadCGIEvent::reader()
{
    return (this->reader);
}
*/



