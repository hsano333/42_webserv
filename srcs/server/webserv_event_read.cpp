#include "webserv_read_event.hpp"

WebservReadEvent::WebservReadEvent() 
                                        :
                                        fd(FileDiscriptor::from_int(0)),
                                        event_type(READ_EVENT),
                                        timeout_count(0)
{
    ;
}

WebservReadEvent::WebservReadEvent(FileDiscriptor fd, IReader& ireader)
    :
        fd(fd),
        event_type(READ_EVENT),
        timeout_count(0),
        ireader(ireader)
{
    ;
}

WebservReadEvent::~WebservReadEvent()
{
    ;
}

EWebservEvent WebservReadEvent::which()
{
    return (READ_EVENT);
}


FileDiscriptor WebservReadEvent::get_fd()
{
    return (this->fd);
}


void WebservReadEvent::increase_and_check_timeout_count(int count)
{
    this->timeout_count += count;
    if(this->timeout_count >= TIMEOUT){
        DEBUG("Exceed Timeout:" + Utility::to_string(TIMEOUT) + "sec");

    }
}

void WebservReadEvent::read(  )
{
    iread.read(fd, );

}
