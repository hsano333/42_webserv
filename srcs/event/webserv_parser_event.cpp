#include "webserv_parser_event.hpp"
#include "webserv_event.hpp"

WebservParserEvent::WebservParserEvent(
                            FileDiscriptor fd,
                            Request *req)
                            :
                            fd_(fd),
                            req_(req),
                            res_(NULL),
                            file_(NULL),
                            source_file(NULL),
                            destination_file(NULL),
                            timeout_count_(0),
                            is_completed_(false)
{

};

WebservParserEvent::~WebservParserEvent()
{
};

WebservParserEvent *WebservParserEvent::from_event(WebservEvent *event)
{
    return (new WebservParserEvent(event->fd(), event->req()));
};

EWebservEvent WebservParserEvent::which()
{
    return (PARSER_EVENT);
}

WebservEvent* WebservParserEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservParserEvent::make_next_event");
    return (event_factory->make_application_event(event));
}

E_EpollEvent WebservParserEvent::get_next_epoll_event()
{
    return (EPOLL_NONE);
}

FileDiscriptor WebservParserEvent::fd()
{
    return (this->fd_);
}

Request *WebservParserEvent::req()
{
    return (req_);
}

Response *WebservParserEvent::res()
{
    return (this->res_);
}

File *WebservParserEvent::src()
{
    return (this->source_file);
}

File *WebservParserEvent::dst()
{
    return (this->destination_file);
}

bool WebservParserEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservParserEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}

void WebservParserEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservParserEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservParserEvent::timeout_count()
{
    return (this->timeout_count_);
}

/*
void WebservParserEvent::set_response(Response *res)
{
    this->res_ = res;
}


void WebservParserEvent::set_file(File *file)
{
    this->file_ = file;
}

File *WebservParserEvent::file()
{
    return (this->file_);
}
*/

void WebservParserEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservParserEvent::cgi_event()
{
    return (this->cgi_event_);
}
