#include "webserv_application_event.hpp"
#include "webserv_event.hpp"

WebservApplicationEvent::WebservApplicationEvent(
                            FileDiscriptor fd,
                            Request *req)
                            :
                            fd_(fd),
                            req_(req),
                            res_(NULL),
                            file_(NULL),
                            timeout_count_(0),
                            is_completed_(false),
                            cgi_event_(NULL)
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
    //todo
    if(event->cgi_event()){
        return (event_factory->make_read_event_from_event(event));
    }else{
        return (event_factory->make_write_event(event, event->res()));
    }
}

E_EpollEvent WebservApplicationEvent::get_next_epoll_event()
{

    if (this->is_completed_){
        if(this->cgi_event() == NULL){
            return (EPOLL_WRITE);

        }else{
            return (EPOLL_CGI_OUT);
        }
    }else{
        return (EPOLL_READ);
    }
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

bool WebservApplicationEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservApplicationEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
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


void WebservApplicationEvent::set_file(File *file)
{
    this->file_ = file;
}

File *WebservApplicationEvent::file()
{
    return (this->file_);
}

void WebservApplicationEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservApplicationEvent::cgi_event()
{
    return (this->cgi_event_);
}
