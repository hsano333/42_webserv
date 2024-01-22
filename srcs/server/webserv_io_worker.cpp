#include "webserv_io_worker.hpp"
#include "http_exception.hpp"
#include "connection_exception.hpp"
#include <string.h>

WebservIOWorker::WebservIOWorker(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager,
            SocketWriter *writer,
            SocketReader *reader
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager),
        writer(writer),
        reader(reader)

{
    ;
}

WebservIOWorker::~WebservIOWorker()
{
    ;
}

void WebservIOWorker::work(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservIOWorker::work");
    File *source = event->src();
    File *destination = event->dst();
    if(source == NULL || destination == NULL){
        ERROR("WebservReceiver::recv():  source is NULL");
        throw HttpException("500");
    }
    source->open();
    destination->open();
    event->set_completed(false);
    char buf[MAX_READ_SIZE+1];
    ssize_t read_size_total = 0;
    while(1)
    {

        char *buf_p = &(buf[0]);
        ssize_t read_size = source->read(&buf_p, MAX_READ_SIZE);
        MYINFO("MYINFO::read size=" + Utility::to_string(read_size));
        if(read_size <= 0){
            MYINFO("MYINFO::read end");
            event->set_completed(true);
            break;
        }
        read_size_total += read_size;
        buf_p[read_size] = '\0';
        printf("buf_p=%s\n", buf_p);
        ssize_t write_size = destination->write(&buf_p, read_size);
        if(write_size <= 0){
            MYINFO("MYINFO::write end");
            event->set_completed(false);
            source->save(buf_p, read_size);
            break;
        }else{
            MYINFO("Write OK::" + Utility::to_string(write_size));
        }
    }
    //source->close();
    //destination->close();
}

