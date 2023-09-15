
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservNothingEvent::WebservNothingEvent() : timeout_count_(0)
{
    ;
}


WebservNothingEvent::~WebservNothingEvent()
{
    ;
}

EWebservEvent WebservNothingEvent::which()
{
    return (NOTHING_EVENT);
}

FileDiscriptor WebservNothingEvent::get_fd()
{
    return (FileDiscriptor::from_int(0));
}

Request *WebservNothingEvent::req()
{
    return (NULL);
}

Response *WebservNothingEvent::res()
{
    return (NULL);
}

void WebservNothingEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservNothingEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservNothingEvent::timeout_count()
{
    return (this->timeout_count_);
}
