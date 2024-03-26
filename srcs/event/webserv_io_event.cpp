#include "webserv_io_event.hpp"

bool write_buffer_data(WebservEntity *entity, char *buf_p)
{
    DEBUG("write_buffer_data()");
    //char buf[MAX_READ_SIZE+1];
    //char *buf_p = &(buf[0]);
    WebservFile *destination = entity->io().destination();
    size_t loaded_size = entity->io().load(&buf_p);
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("-----------------------------------------\n");
    printf("write buffer test buf=[%s]\n\n", buf_p);
    printf("buffer size =[%zu]\n\n", loaded_size);
    if(loaded_size != 0){
        ssize_t write_size = destination->write(&buf_p, loaded_size);
        if(write_size < 0){
            return (false);
        }else if(loaded_size == (size_t)write_size){
            entity->io().clear_tmp_data();
        }else if(loaded_size > (size_t)write_size){
            entity->io().save(buf_p, write_size, loaded_size);
        }
    }
    return (true);
}
