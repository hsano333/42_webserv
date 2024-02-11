
#include "webserv_keep_alive_event.hpp"
#include "global.hpp"

WebservKeepAliveEvent::WebservKeepAliveEvent() 
    :
        timeout_count_(0)
        //is_completed_(false)
{
    this->fd_ = FileDiscriptor::from_int(0);
}

WebservKeepAliveEvent::WebservKeepAliveEvent(FileDiscriptor fd) : timeout_count_(0), fd_(fd)
{
    ;
}


WebservKeepAliveEvent::~WebservKeepAliveEvent()
{
    ;
}

WebservKeepAliveEvent *WebservKeepAliveEvent::from_fd(FileDiscriptor fd)
{
    WebservKeepAliveEvent *event = new WebservKeepAliveEvent(fd);
    return (event);
}

EWebservEvent WebservKeepAliveEvent::which()
{
    return (KEEPA_ALIVE_EVENT);
}

WebservEvent* WebservKeepAliveEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    WARNING("WebservKeepAliveEvent::make_next_event() there is no next event");
    //
    return (event_factory->make_io_socket_event_as_read(event));
    //return (NULL);
    //return (event);
}

E_EpollEvent WebservKeepAliveEvent::get_next_epoll_event()
{
    return (EPOLL_READ);
}

FileDiscriptor &WebservKeepAliveEvent::fd()
{
    return (fd_);
}

Request *WebservKeepAliveEvent::req()
{
    return (NULL);
}

Response *WebservKeepAliveEvent::res()
{
    return (NULL);
}

File *WebservKeepAliveEvent::src()
{
    return (NULL);
}

File *WebservKeepAliveEvent::dst()
{
    return (NULL);
}

void WebservKeepAliveEvent::set_src(File *file)
{
    (void)file;
    //this->source_file = file;
}

void WebservKeepAliveEvent::set_dst(File *file)
{
    (void)file;
    //this->destination_file = file;
}

bool WebservKeepAliveEvent::is_completed()
{
    return (true);
}
void WebservKeepAliveEvent::set_completed(bool flag)
{
    (void)flag;
    //this->is_completed_ = flag;
}

void WebservKeepAliveEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservKeepAliveEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservKeepAliveEvent::timeout_count()
{
    return (this->timeout_count_);
}



void WebservKeepAliveEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservKeepAliveEvent::cgi_event()
{
    return (this->cgi_event_);
}

void WebservKeepAliveEvent::switching_io(uint32_t epoll_event)
{
    (void)epoll_event;
}

void WebservKeepAliveEvent::set_write_io(File *src, File *dst)
{
    (void)src;
    (void)dst;
}

void WebservKeepAliveEvent::set_read_io(File *src, File *dst)
{
    (void)src;
    (void)dst;
}

FileDiscriptor  &WebservKeepAliveEvent::get_write_fd()
{
    return (this->write_fd_);
}

FileDiscriptor  &WebservKeepAliveEvent::get_read_fd()
{
    return (this->read_fd_);
}

/*
FileDiscriptor  &WebservKeepAliveEvent::get_socket_fd()
{
    return (this->sock_fd_);
}
*/

WebservEntity*WebservKeepAliveEvent::entity()
{
    return (this->entity_);
}
