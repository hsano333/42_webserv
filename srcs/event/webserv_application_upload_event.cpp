#include "webserv_application_upload_event.hpp"
#include "webserv_event.hpp"
#include "webserv_io_event.hpp"
#include "socket_reader.hpp"
#include "global.hpp"
#include "application_result.hpp"
#include "webserv_file_factory.hpp"

WebservApplicationUploadEvent::WebservApplicationUploadEvent()
{
}


WebservApplicationUploadEvent::~WebservApplicationUploadEvent()
{
    ;
}

WebservApplicationUploadEvent *WebservApplicationUploadEvent::singleton = NULL;
WebservApplicationUploadEvent *WebservApplicationUploadEvent::get_instance()
{
    if (WebservApplicationUploadEvent::singleton == NULL){
        singleton = new WebservApplicationUploadEvent();
    }
    return (singleton);
}

void WebservApplicationUploadEvent::delete_myself()
{
    if(WebservApplicationUploadEvent::singleton)
        delete WebservApplicationUploadEvent::singleton;
}



WebservEvent *WebservApplicationUploadEvent::from_event(WebservEvent *event, WebservFile *src, WebservFile *dst)
{
    DEBUG("WebservApplicationUploadEvent::from_event");
    WebservApplicationUploadEvent *upload_event = WebservApplicationUploadEvent::get_instance();
    WebservEvent *new_event = new WebservEvent(upload_event, io_work<WebservApplicationUploadEvent>, event->entity());

    new_event->entity()->io().set_read_io(src, dst);
    new_event->entity()->io().switching_io(EPOLLIN);
    new_event->entity()->io().set_total_write_size(0);
    return (new_event);
}

WebservEvent* WebservApplicationUploadEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationUploadEvent::make_next_event");

    if(event->entity()->app_result()->status_code() == 200){
        DEBUG("WebservApplicationUploadEvent:: 200 OK");
        return event_factory->make_making_response_event(event, NULL, NULL);
    }else{
        DEBUG("WebservApplicationUploadEvent:: 207 Multi Response");
        WebservFileFactory *file_factory = WebservFileFactory::get_instance();
        ApplicationResult *result = event->entity()->app_result();
        result->add_header(TRANSFER_ENCODING, TRANSFER_ENCODING_CHUNKED);
        WebservFile *multi_file = event->entity()->io().destination();
        WebservFile *chunk_file = file_factory->make_socket_chunk_file_for_write(event->entity()->fd(), multi_file);
        chunk_file->set_chunk(true);
        result->set_file(chunk_file);
        return event_factory->make_making_response_event(event, chunk_file, NULL);
    }
}

E_EpollEvent WebservApplicationUploadEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    if(event->entity()->completed()){
        return (EPOLL_NONE);
    }
    return (EPOLL_READ);
}

void WebservApplicationUploadEvent::check_completed(WebservEntity * entity)
{
    WebservFile *file = entity->io().destination();

    size_t total_size = entity->io().total_write_size();
    size_t content_length = entity->request()->header().get_content_length();
    bool is_completed = file->completed();

    if((file->is_chunk() == false && content_length == total_size) && is_completed){
        is_completed = true;
    }else if(file->is_chunk() == true  && is_completed){
        is_completed = true;
    }else{
        is_completed = false;
    }
    DEBUG("WebservApplicationUploadEvent::total_size :" + Utility::to_string(total_size));
    entity->set_completed(is_completed);
    entity->request()->set_read_completed(true);

    if(is_completed){
        char *tmp = NULL;
            // 一度目だけステータスコードを読み取れる。
            // ２回目以降は読み取れなくなる
        int status_code = file->read(&tmp, 0);
        entity->app_result()->set_status_code(status_code);
    }
}

