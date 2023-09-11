#include "webserv_timeout_event.hpp"

WebservTimeoutEvent::WebservTimeoutEvent() 
                                        :
                                        fd(FileDiscriptor::from_int(0)),
                                        event_type(TIMEOUT_EVENT),
                                        timeout_count(0)
{
    ;
}

WebservTimeoutEvent::~WebservTimeoutEvent()
{
    ;
}

EWebservEvent WebservTimeoutEvent::which()
{
    return (this->event_type);
}


FileDiscriptor WebservTimeoutEvent::get_fd()
{
    return (this->fd);
}

Request *WebservTimeoutEvent::req()
{
    return (NULL);
}

Response *WebservTimeoutEvent::res()
{
    return (NULL);
}


void WebservTimeoutEvent::increase_and_check_timeout_count(int count)
{
    this->timeout_count += count;
    if(this->timeout_count >= TIMEOUT){
        DEBUG("Exceed Timeout:" + Utility::to_string(TIMEOUT) + "sec");

    }
}

