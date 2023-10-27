
#include "webserv_keep_alive_event.hpp"
#include "global.hpp"

WebservKeepAliveEvent::WebservKeepAliveEvent() 
    :
        timeout_count_(0)
        //is_end_(false)
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
    WARNING("WebservTimeoutEvent::make_next_event() there is no next event");
    return (event);
}

E_EpollEvent WebservKeepAliveEvent::get_next_epoll_event()
{
    return (EPOLL_CONTINUE);
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

bool WebservKeepAliveEvent::is_end()
{
    return (true);
}
void WebservKeepAliveEvent::set_end(bool flag)
{
    (void)flag;
    //this->is_end_ = flag;
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
