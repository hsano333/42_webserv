
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservCleanEvent::WebservCleanEvent(
                            FileDiscriptor fd,
                            Request *req,
                            Response *res
                            ):
                            fd_(fd),
                            req_(req),
                            res_(res),
                            timeout_count_(0),
                            force_close(false)
{
    ;
}

WebservCleanEvent::~WebservCleanEvent()
{
    ;
}

EWebservEvent WebservCleanEvent::which()
{
    return (CLEAN_EVENT);
}

FileDiscriptor WebservCleanEvent::fd()
{
    return (this->fd_);
}

Request *WebservCleanEvent::req()
{
    return (this->req_);
}

Response *WebservCleanEvent::res()
{
    return (this->res_);
}

void WebservCleanEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservCleanEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservCleanEvent::timeout_count()
{
    return (this->timeout_count_);
}

bool WebservCleanEvent::is_force_close()
{
    return (this->force_close);
}


WebservCleanEvent *WebservCleanEvent::from_webserv_event(WebservEvent *event, bool force_close)
{
    WebservCleanEvent *new_event = new WebservCleanEvent(event->fd(), event->req(), event->res());
    new_event->force_close = force_close;
    return (new_event);
}

