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

// event内にあるsrcファイルから読み出し、dstファイルに書き出す
// 終了条件は、MAX_READ_SIZE以上のデータを読み出す。
// あるいは、改行文字が2つ連続したとき
// この判定はset_completed()で実施する
void WebservIOWorker::work(WebservEvent *event)
{
    (void)event;

    DEBUG("WebservIOWorker::work");
    //test 後で消す
    event->entity()->config()->check();

    WebservEntity *entity = event->entity();

    File *source = entity->io().source();
    File *destination = entity->io().destination();
    printf("souce=%p\n", source);
    printf("destination=%p\n", destination);
    printf("souce=%p\n", source);
    printf("souce=%p\n", source);
    printf("destination=%p\n", destination);
    printf("destination=%p\n", destination);
    if(source == NULL || destination == NULL){
        ERROR("WebservReceiver::recv():  either source or destination is NULL");
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

/*
void io_work(WebservIOEvent *event)
{
    (void)event;

    DEBUG("WebservIOWorker::work");
    //test 後で消す
    //event->entity()->config()->check();

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
}

*/
