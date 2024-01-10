
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservNothingEvent::WebservNothingEvent()
    :
        timeout_count_(0)
        //req_(NULL),
        //res_(NULL),
        //source_file(NULL),
        //destination_file(NULL),
        //is_completed_(false)
{
    this->fd_ = FileDiscriptor::from_int(0);
}

WebservNothingEvent::WebservNothingEvent(FileDiscriptor fd) : timeout_count_(0), fd_(fd)
{
    ;
}


WebservNothingEvent::~WebservNothingEvent()
{
    ;
}

WebservNothingEvent *WebservNothingEvent::from_fd(FileDiscriptor fd)
{
    WebservNothingEvent *event = new WebservNothingEvent(fd);
    return (event);
}

EWebservEvent WebservNothingEvent::which()
{
    return (NOTHING_EVENT);
}

WebservEvent* WebservNothingEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    (void)event_factory;
    (void)event;
    WARNING("WebservNothingEvent::make_next_event() there is no next event");
    return (NULL);
}

E_EpollEvent WebservNothingEvent::get_next_epoll_event()
{
    return (EPOLL_NONE);
}


FileDiscriptor WebservNothingEvent::fd()
{
    return (fd_);
}

Request *WebservNothingEvent::req()
{
    return (NULL);
}

Response *WebservNothingEvent::res()
{
    return (NULL);
}

File *WebservNothingEvent::src()
{
    return (NULL);
}

File *WebservNothingEvent::dst()
{
    return (NULL);
}

void WebservNothingEvent::set_src(File *file)
{
    (void)file;
    //this->source_file = file;
}

void WebservNothingEvent::set_dst(File *file)
{
    (void)file;
    //this->destination_file = file;
}

bool WebservNothingEvent::is_completed()
{
    return (true);
    //return (this->is_completed_);
}
void WebservNothingEvent::set_completed(bool flag)
{
    (void)flag;
    //this->is_completed_ = flag;
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

void WebservNothingEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservNothingEvent::cgi_event()
{
    return (this->cgi_event_);
}


