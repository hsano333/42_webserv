#include "webserv_read_event.hpp"
#include "read_socket.hpp"
#include "read_raw.hpp"
#include "global.hpp"

WebservReadEvent::WebservReadEvent()
                                        :
                                        fd(FileDiscriptor::from_int(0)),
                                        event_type(READ_EVENT),
                                        timeout_count(0),
                                        ireader(NULL)
{
}

WebservReadEvent::WebservReadEvent(FileDiscriptor fd)
    :
        fd(fd),
        event_type(READ_EVENT),
        timeout_count(0),
        ireader(new ReadSocket())
        //iread()
{
    ;
}

WebservReadEvent::WebservReadEvent(FileDiscriptor fd, IReader *ireader)
    :
        fd(fd),
        event_type(READ_EVENT),
        timeout_count(0),
        ireader(ireader)
        //iread()
{
    ;
}


WebservReadEvent::~WebservReadEvent()
{
    ;
}

EWebservEvent WebservReadEvent::which()
{
    return (this->event_type);
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

int WebservReadEvent::read(char *buf, size_t size)
{

    MYINFO("WebservReadEvent::read() fd:" + Utility::to_string(fd));
    return this->ireader->read(fd, buf, size);
}
