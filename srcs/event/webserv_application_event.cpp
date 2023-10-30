#include "webserv_application_event.hpp"
#include "webserv_event.hpp"

WebservApplicationEvent::WebservApplicationEvent(
                            FileDiscriptor fd,
                            Request *req)
                            :
                            fd_(fd),
                            req_(req),
                            res_(NULL),
                            timeout_count_(0),
                            is_end_(false)
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

WebservEvent* WebservApplicationEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationEvent::make_next_event");
    return (event_factory->make_write_event(event, event->res()));
}

E_EpollEvent WebservApplicationEvent::get_next_epoll_event()
{
    return (EPOLL_WRITE);
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
    return (this->res_);
}

bool WebservApplicationEvent::is_end()
{
    return (this->is_end_);
}
void WebservApplicationEvent::set_end(bool flag)
{
    this->is_end_ = flag;
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
