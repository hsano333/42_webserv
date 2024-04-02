#ifndef WEBSERV_IO_EVENT_HPP
#define WEBSERV_IO_EVENT_HPP
#include "webserv_entity.hpp"
#include "file_discriptor.hpp"
#include "webserv_file.hpp"
#include "webserv_io_cgi_event.hpp"

class WebservEntity;
bool write_buffer_data(WebservEntity *entity, char *buf_p);

template<typename EventT>
bool io_work(EventT *event, WebservEntity *entity)
{
    (void)event;
    entity->io().total_write_size();

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
    //char *buf_init = &(buf[0]);
    ssize_t read_size_total = 0;

    char *buf_p = (buf);
    size_t load_size = entity->io().load(&buf_p);
    
    while(1)
    {
        entity->io().total_write_size();
        //DEBUG("WebservIOWorker::work No.3");
        buf_p = &(buf[load_size]);
        ssize_t read_size = source->read(&buf_p, MAX_READ_SIZE - load_size);
        MYINFO("MYINFO::read size=" + Utility::to_string(read_size));
        if(read_size <= 0){
            MYINFO("MYINFO::read end");
            break;
        }
        read_size_total += read_size;
        buf_p[read_size] = '\0';
        //printf("\n\n size=%zu, webserv_io_event read buf=[%s]\n", read_size, buf);
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("\nwrite buffer test buf=[");
    size_t tmp_size = read_size;
    int i=0;
    while(tmp_size > 0){
        printf("%c", (buf_p)[i]);
        i++;
        tmp_size--;
    }
    printf("]\nbuffer size =[%zu]\n\n", read_size);


        MYINFO("io_event ");
        ssize_t write_size = destination->write(&buf_p, read_size);
        if(write_size <= 0){
            MYINFO("write end");
            entity->io().save(buf_p, 0, read_size);
            break;
        }else if(read_size > write_size){
            MYINFO("read_size=" + Utility::to_string(read_size));
            MYINFO("write_size=" + Utility::to_string(write_size));
            printf("read_size=%s\n" , Utility::to_string(read_size).c_str());
            printf("write_size=%s\n" , Utility::to_string(write_size).c_str());
            //entity->io().save(*buf, write_size, read_size);
            size_t diff = read_size - write_size;
            Utility::memcpy(buf, &(buf[write_size]), diff);
            entity->io().add_total_write_size(write_size);
            load_size = read_size - write_size;
            break;
        }else{
            MYINFO("Write OK::" + Utility::to_string(write_size));
            entity->io().add_total_write_size(write_size);
            load_size = 0;
        }
    }
    DEBUG("WebservIOWorker::check completed");
    event->check_completed(entity);
    entity->io().total_write_size();
    return (entity->completed());
}

#endif
