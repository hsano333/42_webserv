#include "webserv_application_event.hpp"
#include "webserv_event.hpp"

WebservApplicationEvent::WebservApplicationEvent(
                            FileDiscriptor fd,
                            Request *req)
                            :
                            fd(fd),
                            req_(req),
                            timeout_count_(0)
{

};

WebservApplicationEvent::~WebservApplicationEvent()
{
    //delete req;
};

WebservApplicationEvent *WebservApplicationEvent::from_event(WebservEvent *event)
{
    return (new WebservApplicationEvent(event->get_fd(), event->req()));
};

EWebservEvent WebservApplicationEvent::which()
{
    return (APPLICATION_EVENT);
}

FileDiscriptor WebservApplicationEvent::get_fd()
{
    return (this->fd);
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
