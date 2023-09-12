
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservCleanEvent::WebservCleanEvent(
                            FileDiscriptor fd,
                            Request *req,
                            Response *res
                            ):
                            fd(fd),
                            req_(req),
                            res_(res)
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

FileDiscriptor WebservCleanEvent::get_fd()
{
    return (this->fd);
}

Request *WebservCleanEvent::req()
{
    return (this->req_);
}

Response *WebservCleanEvent::res()
{
    return (this->res_);
}

void WebservCleanEvent::increase_and_check_timeout_count(int count)
{
    (void)(count);
}

WebservCleanEvent *WebservCleanEvent::from_webserv_event(WebservEvent *event)
{
    WebservCleanEvent *new_event = new WebservCleanEvent(event->get_fd(), event->req(), event->res());
    return (new_event);
}

