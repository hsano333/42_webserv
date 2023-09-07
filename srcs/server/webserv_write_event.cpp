#include "webserv_write_event.hpp"

WebservWriteEvent::WebservWriteEvent() 
                                        :
                                        fd(FileDiscriptor::from_int(0)),
                                        event_type(READ_EVENT),
                                        timeout_count(0)
{
    ;
}

WebservWriteEvent::WebservWriteEvent(FileDiscriptor fd)
    :
        fd(fd),
        event_type(READ_EVENT),
        timeout_count(0)
{
    ;
}

WebservWriteEvent::~WebservWriteEvent()
{
    ;
}

EWebservEvent WebservWriteEvent::which()
{
    return (this->event_type);
}


FileDiscriptor WebservWriteEvent::get_fd()
{
    return (this->fd);
}


void WebservWriteEvent::increase_and_check_timeout_count(int count)
{
    this->timeout_count += count;
    if(this->timeout_count >= TIMEOUT){
        DEBUG("Exceed Timeout:" + Utility::to_string(TIMEOUT) + "sec");

    }
}

