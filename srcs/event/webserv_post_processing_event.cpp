
#include "webserv_post_processing_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservPostProcessingEvent::WebservPostProcessingEvent(
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

WebservPostProcessingEvent::~WebservPostProcessingEvent()
{
    ;
}

EWebservEvent WebservPostProcessingEvent::which()
{
    return (POST_PROCESSING_EVENT);
}

FileDiscriptor WebservPostProcessingEvent::get_fd()
{
    return (FileDiscriptor::from_int(0));
}

Request *WebservPostProcessingEvent::req()
{
    return (this->req_);
}

Response *WebservPostProcessingEvent::res()
{
    return (this->res_);
}

void WebservPostProcessingEvent::increase_and_check_timeout_count(int count)
{
    (void)(count);
}

WebservPostProcessingEvent *WebservPostProcessingEvent::from_webserv_event(WebservEvent *event)
{
    WebservPostProcessingEvent *new_event = new WebservPostProcessingEvent(event->get_fd(), event->req(), event->res());
    return (new_event);
}

