#include "webserv_application_event.hpp"
#include "webserv_event.hpp"

WebservApplicationEvent::WebservApplicationEvent(Request *req) : 
                                                req(req)
{

};

WebservApplicationEvent::~WebservApplicationEvent()
{
    delete req;
};

WebservApplicationEvent *WebservApplicationEvent::from_request(FileDiscriptor fd, Request *req)
{
    WebservApplicationEvent *event = new WebservApplicationEvent(req);
    event->fd = fd;
    return (event);
};

EWebservEvent WebservApplicationEvent::which()
{
    return (APPLICATION_EVENT);
}

FileDiscriptor WebservApplicationEvent::get_fd()
{
    return (this->fd);
}

void WebservApplicationEvent::increase_and_check_timeout_count(int count)
{
    this->timeout_count += count;
    if(this->timeout_count >= TIMEOUT){
        DEBUG("Exceed Timeout:" + Utility::to_string(TIMEOUT) + "sec");
    }
}
