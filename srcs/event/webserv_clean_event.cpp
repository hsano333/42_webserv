
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
                            force_close(false),
                            is_completed_(false)
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

WebservEvent* WebservCleanEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    if (this->force_close){
        MYINFO("WebservCleanEvent::make_next_event() NULL");
        //delete this;
        return (NULL);
    }else{
        MYINFO("WebservCleanEvent::make_next_event() Read Event");
        printf("WebservCleanEvent::make_next_event event=%p\n", event);
        return (event_factory->make_read_event_from_event(event));
    }
    (void)event_factory;
    (void)event;
    return (NULL);
}

E_EpollEvent WebservCleanEvent::get_next_epoll_event()
{
    if (this->force_close){
        return (EPOLL_CLOSE);
    }else{
        return (EPOLL_READ);
    }
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

File *WebservCleanEvent::src()
{
    return (this->source_file);
}

File *WebservCleanEvent::dst()
{
    return (this->destination_file);
}

void WebservCleanEvent::set_null_res_and_req()
{
    this->res_ = NULL;
    this->req_ = NULL;
}

bool WebservCleanEvent::is_completed()
{
    return (this->is_completed_);
}

void WebservCleanEvent::set_force_close(bool flag)
{
    this->force_close = flag;
}

void WebservCleanEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
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


void WebservCleanEvent::clean_res_and_req()
{
    delete this->res_;
    delete this->req_;

    this->res_ = NULL;
    this->req_ = NULL;

}

void WebservCleanEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservCleanEvent::cgi_event()
{
    return (this->cgi_event_);
}
