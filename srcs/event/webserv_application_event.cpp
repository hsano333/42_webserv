#include "webserv_application_event.hpp"
#include "webserv_event.hpp"

WebservApplicationEvent::WebservApplicationEvent(
                            FileDiscriptor fd,
                            Request *req)
                            :
                            fd_(fd),
                            req_(req),
                            res_(NULL),
                            timeout_count_(0)
{

};

WebservApplicationEvent::~WebservApplicationEvent()
{
};

WebservApplicationEvent *WebservApplicationEvent::from_event(WebservEvent *event)
{
    return (new WebservApplicationEvent(event->fd(), event->req()));
};

EWebservEvent WebservApplicationEvent::which()
{
    return (APPLICATION_EVENT);
}

FileDiscriptor WebservApplicationEvent::fd()
{
    return (this->fd_);
}

Request *WebservApplicationEvent::req()
{
    return (req_);
}

Response *WebservApplicationEvent::res()
{
    return (NULL);
}

void WebservApplicationEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservApplicationEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservApplicationEvent::timeout_count()
{
    return (this->timeout_count_);
}

void WebservApplicationEvent::set_response(Response *res)
{
    this->res_ = res;
}
