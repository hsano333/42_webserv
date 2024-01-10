
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
    return (event_factory->make_read_event_from_event(event));
    //return (NULL);
    //return (event);
}

E_EpollEvent WebservKeepAliveEvent::get_next_epoll_event()
{
    return (EPOLL_READ);
}

FileDiscriptor WebservKeepAliveEvent::fd()
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



