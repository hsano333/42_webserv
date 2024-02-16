#ifndef WEBSERV_IO_WORKER_HPP
#define WEBSERV_IO_WORKER_HPP

#include "webserv_event.hpp"
#include "webserv_read_event.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"
#include "socket_writer.hpp"
#include "socket_reader.hpp"

class WebservIOWorker
{
    public:
        //WebservReader();
        WebservIOWorker(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    SocketWriter *writer,
                    SocketReader *reader
                );
        ~WebservIOWorker();
        //void read(WebservReadEvent *event);
        void work(WebservEvent *event);
        //void recv_continue(WebservEvent *event);
        //bool have_executable_events();
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        SocketWriter *writer;
        SocketReader *reader;

};


/*
template <typename EventPointer>
void io_work(EventPointer event)
{
    (void)event;

    DEBUG("WebservIOWorker::work");
    //test 後で消す
    event->entity()->config()->check();

    WebservEntity *entity = event->entity();

    File *source = entity->io()->source();
    File *destination = entity->io()->destination();
    if(source == NULL || destination == NULL){
        ERROR("WebservReceiver::recv():  source is NULL");
        throw HttpException("500");
    }
    source->open();
    destination->open();
    entity->set_completed(false);
    char buf[MAX_READ_SIZE+1];
    ssize_t read_size_total = 0;
    while(1)
    {
        char *buf_p = &(buf[0]);
        ssize_t read_size = source->read(&buf_p, MAX_READ_SIZE);
        MYINFO("MYINFO::read size=" + Utility::to_string(read_size));
        if(read_size <= 0){
            MYINFO("MYINFO::read end");
            entity->set_completed(true);
            break;
        }
        read_size_total += read_size;
        buf_p[read_size] = '\0';
        printf("buf_p=%s\n", buf_p);
        ssize_t write_size = destination->write(&buf_p, read_size);
        if(write_size <= 0){
            MYINFO("MYINFO::write end");
            entity->set_completed(false);
            source->save(buf_p, read_size);
            break;
        }else{
            MYINFO("Write OK::" + Utility::to_string(write_size));
        }
    }
    //source->close();
    //destination->close();

}
*/


#endif

