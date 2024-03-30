
#include "webserv_application_upload_event.hpp"
#include "webserv_event.hpp"
#include "webserv_io_event.hpp"
#include "socket_reader.hpp"
#include "global.hpp"

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


WebservEvent *WebservApplicationUploadEvent::from_event(WebservEvent *event, WebservFile *src, WebservFile *dst)
{
    DEBUG("WebservApplicationUploadEvent::from_event");
    WebservApplicationUploadEvent *upload_event = WebservApplicationUploadEvent::get_instance();
    WebservEvent *new_event = new WebservEvent(upload_event, io_work<WebservApplicationUploadEvent>, event->entity());

    //Request const *req = event->entity()->request();
    //WebservFile *dst;
    //(void)req;
    //(void)dst;

    //WebservFile *src = this->file_factory->make_socket_file(event->entity()->fd(), socket_writer, socket_reader);
    //WebservFile *read_dst = this->file_factory->make_vector_file(fd_ref, MAX_REAUEST_EXCEPT_BODY);

    new_event->entity()->io().set_read_io(src, dst);
    new_event->entity()->io().set_read_fd(event->entity()->fd());
    new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}

WebservEvent* WebservApplicationUploadEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationUploadEvent::make_next_event");

    //WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    return event_factory->make_making_response_event(event, );
    /*
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    SocketReader *socket_reader = SocketReader::get_instance();
    WebservFile *src;
    if(event->entity()->io().is_read_completed()){
        char *buf_p;
        size_t loaded_size = event->entity()->io().load(&buf_p);
        src = file_factory->make_vector_file(event->entity()->fd(), buf_p, loaded_size);
        //src = file_factory->make_vector_file(event->entity()->fd(), );
    }else{
        src = file_factory->make_socket_file(event->entity()->fd(), NULL, socket_reader);
    }
    return (event_factory->make_io_socket_event_as_read(event, src));
    */
}

E_EpollEvent WebservApplicationUploadEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
}

void WebservApplicationUploadEvent::check_completed(WebservEntity * entity)
{
    WebservFile *file = entity->io().destination();
    DEBUG("WebservApplicationUploadEvent::check_completed :" + Utility::to_string(file->completed()));
    entity->set_completed(file->completed());
}

