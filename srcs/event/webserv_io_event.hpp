#ifndef WEBSERV_IO_EVENT_HPP
#define WEBSERV_IO_EVENT_HPP
#include "webserv_entity.hpp"
#include "file_discriptor.hpp"
#include "webserv_file.hpp"
#include "webserv_io_get_cgi_event.hpp"

class WebservEntity;
bool write_buffer_data(WebservEntity *entity, char *buf_p);

template<typename EventT>
bool io_work(EventT *event, WebservEntity *entity)
{
    (void)event;
    DEBUG("WebservIOWorker::work fd:" + entity->fd().to_string());

    WebservFile *source = entity->io().source();
    WebservFile *destination = entity->io().destination();
    if(source == NULL || destination == NULL){
        ERROR("WebservReceiver::recv():  source is NULL");
        throw HttpException("500");
    }
    source->test();
    source->open();
    destination->open();
    entity->set_completed(false);
    char buf[MAX_READ_SIZE+1];
    ssize_t read_size_total = 0;

    char *buf_p = (buf);
    size_t load_size = 0;
    
    while(1)
    {
        buf_p = &(buf[load_size]);
        DEBUG("io read() read_size:" + Utility::to_string(MAX_READ_SIZE - load_size));
        DEBUG("io read() buffer_size:" + Utility::to_string(load_size));
        //char *buf_p_bk = buf_p;
        ssize_t read_size = source->read(&buf_p, MAX_READ_SIZE - load_size);
        if(read_size <= 0){
            MYINFO("MYINFO::read end");
            break;
        }
        read_size_total += read_size;
        buf_p = &(buf[0]);
        read_size += load_size;
        /*
        size_t tmp_size = read_size;

        // will remove
        for(size_t i=0;i<tmp_size;i++){
            if((buf_p)[i] == '\r'){
                printf("R");
            }else if((buf_p)[i] == '\n'){
                printf("N");
            }else{
                printf("%c", (buf_p)[i]);
            }
        }
        printf("]\nbuffer size =[%zu]\n\n", read_size);
        */


        DEBUG("io write()");
        ssize_t write_size = destination->write(&buf_p, read_size);
        if(write_size <= 0){
            MYINFO("write end");
            entity->io().save(buf_p, 0, read_size);
            break;
        }else if(read_size > write_size){
            MYINFO("read_size=" + Utility::to_string(read_size));
            MYINFO("write_size=" + Utility::to_string(write_size));
            size_t diff = read_size - write_size;
            Utility::memcpy(buf, buf_p, diff);
            entity->io().add_total_write_size(write_size);
            load_size = read_size - write_size;
        }else{
            MYINFO("Write OK::" + Utility::to_string(write_size));
            entity->io().add_total_write_size(write_size);
            load_size = 0;
        }
    }

    DEBUG("WebservIOWorker::check completed");
    event->check_completed(entity);
    DEBUG("WebservIOWorker::check completed No.2");
    return (entity->completed());
}

template<typename EventT>
bool io_work_reverse_io(EventT *event, WebservEntity *entity)
{
    DEBUG("io_work_reverse_io");
    if(entity->io().is_cgi_read()){
        DEBUG("io_work_reverse_io No.1");
        entity->io().switching_io(EPOLLIN);
    }else{
        entity->io().switching_io(EPOLLOUT);
    //}else{
        //entity->io().switching_io(EPOLLIN);
    }
    return (io_work<EventT>(event, entity));
}

template<typename EventT>
bool io_work_with_switching_in(EventT *event, WebservEntity *entity)
{
    entity->io().switching_io(EPOLLIN);
    return (io_work<EventT>(event, entity));
}

template<typename EventT>
bool io_work_with_switching_out(EventT *event, WebservEntity *entity)
{
    entity->io().switching_io(EPOLLOUT);
    return (io_work<EventT>(event, entity));
}

template<typename EventT>
bool io_work_ref(EventT *event, WebservEntity *entity)
{
    (void)event;

    DEBUG("WebservIOWorker::io_work_ref fd:" + entity->fd().to_string());

    WebservFile *source = entity->io().source();
    WebservFile *destination = entity->io().destination();
    if(source == NULL || destination == NULL){
        ERROR("WebservReceiver::recv():  source is NULL");
        throw HttpException("500");
    }
    source->test();
    source->open();
    destination->open();
    entity->set_completed(false);
    char buf[MAX_READ_SIZE+1];
    ssize_t read_size_total = 0;

    char *buf_p = (buf);
    size_t load_size = 0;
    
    while(1)
    {
        buf_p = &(buf[load_size]);
        DEBUG("io read()");
        ssize_t read_size = source->read(&buf_p, MAX_READ_SIZE - load_size);
        if(read_size <= 0){
            MYINFO("MYINFO::read end");
            break;
        }
        read_size_total += read_size;
        read_size += load_size;

        /*
        //will remove
        size_t tmp_size = read_size;
        for(size_t i=0;i<tmp_size;i++){
            if((buf_p)[i] == '\r'){
                printf("R");
            }else if((buf_p)[i] == '\n'){
                printf("N");
            }else{
                printf("%c", (buf_p)[i]);
            }
        }
        printf("]\nbuffer size =[%zu]\n\n", read_size);
        */

        DEBUG("io write()");
        ssize_t write_size = destination->write(&buf_p, read_size);
        if(write_size <= 0){
            MYINFO("write end");
            entity->io().save(buf_p, 0, read_size);
            break;
        }else if(read_size > write_size){
            MYINFO("read_size=" + Utility::to_string(read_size));
            MYINFO("write_size=" + Utility::to_string(write_size));
            size_t diff = read_size - write_size;
            Utility::memcpy(buf, buf_p, diff);
            entity->io().add_total_write_size(write_size);
            load_size = read_size - write_size;
        }else{
            MYINFO("Write OK::" + Utility::to_string(write_size));
            entity->io().add_total_write_size(write_size);
            load_size = 0;
        }
    }

    DEBUG("WebservIOWorker::check completed");
    event->check_completed(entity);
    return (entity->completed());
}

#endif
