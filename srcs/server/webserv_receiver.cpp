#include "webserv_receiver.hpp"
#include "http_exception.hpp"
#include "connection_exception.hpp"
#include <string.h>

WebservReceiver::WebservReceiver(
            IOMultiplexing *io_multi_controller,
            FDManager *fd_manager,
            EventManager *event_manager,
            SocketReader *reader
        ):
        io_multi_controller(io_multi_controller),
        fd_manager(fd_manager),
        event_manager(event_manager),
        reader(reader)

{
    ;
}

WebservReceiver::~WebservReceiver()
{
    ;
}

void WebservReceiver::recv(WebservEvent *event)
{
    DEBUG("WebservReader::read()");
    WebservReadEvent *read_event = static_cast<WebservReadEvent*>(event);
    File *source = read_event->src();
    File *destination = read_event->dst();
    if(source == NULL || destination == NULL){
        ERROR("WebservReceiver::recv():  source is NULL");
        throw HttpException("500");
    }
    source->open();
    destination->open();
    event->set_completed(false);
    char buf[MAX_READ_SIZE+1];
    char *buf_p = &(buf[0]);
    //bool completed;
    ssize_t read_size_total = 0;
    while(1)
    {
        ssize_t read_size = source->read(&buf_p, MAX_READ_SIZE);
        if(read_size <= 0){
            event->set_completed(true);
            break;
        }
        read_size_total += read_size;
        ssize_t write_size = destination->write(&buf_p, read_size);
        if(write_size <= 0){
            event->set_completed(false);
            break;
        }
    }
    source->close();
    destination->close();
    //event->set_completed(completed);
    //event->set_read_size(read_size_total);
}
