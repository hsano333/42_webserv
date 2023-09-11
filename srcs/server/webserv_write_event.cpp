#include "webserv_write_event.hpp"
#include "write_socket.hpp"


WebservWriteEvent::WebservWriteEvent() 
                                        :
                                        res(new Response()),
                                        fd(FileDiscriptor::from_int(0)),
                                        timeout_count(0),
                                        writer(new WriteSocket())
{
    ;
}

WebservWriteEvent::WebservWriteEvent(FileDiscriptor fd)
    :
        res(new Response()),
        fd(fd),
        timeout_count(0),
        writer(new WriteSocket())
{
    ;
}

WebservWriteEvent::~WebservWriteEvent()
{
    ;
}

WebservWriteEvent *WebservWriteEvent::from_fd(FileDiscriptor fd)
{
    WebservWriteEvent *event = new WebservWriteEvent(fd);
    return (event);
}


EWebservEvent WebservWriteEvent::which()
{
    return (WRITE_EVENT);
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

int WebservWriteEvent::write(char const *buf, size_t size)
{

    //char buf[] = "HTTP/1.1 200 OK";
    return (this->writer->write(fd, buf, size));
}
