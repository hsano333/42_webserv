#include "webserv_timeout_event.hpp"

WebservTimeoutEvent::WebservTimeoutEvent() 
                                        :
                                        fd_(FileDiscriptor::from_int(0)),
                                        event_type(TIMEOUT_EVENT),
                                        timeout_count_(0),
                                        is_end_(false)
{
    ;
}

WebservTimeoutEvent::~WebservTimeoutEvent()
{
    ;
}

EWebservEvent WebservTimeoutEvent::which()
{
    return (this->event_type);
}

WebservEvent* WebservTimeoutEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    WARNING("WebservTimeoutEvent::make_next_event() there is no next event");
    return (NULL);
}

E_EpollEvent WebservTimeoutEvent::get_next_epoll_event()
{
    return (EPOLL_NONE);
}


FileDiscriptor WebservTimeoutEvent::fd()
{
    return (this->fd_);
}

Request *WebservTimeoutEvent::req()
{
    return (NULL);
}

Response *WebservTimeoutEvent::res()
{
    return (NULL);
}

bool WebservTimeoutEvent::is_end()
{
    return (this->is_end_);
}
void WebservTimeoutEvent::set_end(bool flag)
{
    this->is_end_ = flag;
}


void WebservTimeoutEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservTimeoutEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservTimeoutEvent::timeout_count()
{
    return (this->timeout_count_);
}

