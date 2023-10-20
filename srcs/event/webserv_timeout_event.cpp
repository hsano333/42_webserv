#include "webserv_timeout_event.hpp"

WebservTimeoutEvent::WebservTimeoutEvent() 
                                        :
                                        fd_(FileDiscriptor::from_int(0)),
                                        event_type(TIMEOUT_EVENT),
                                        timeout_count_(0)
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


void WebservTimeoutEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservTimeoutEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservTimeoutEvent::timeout_count()
{
    return (this->timeout_count_);
}

