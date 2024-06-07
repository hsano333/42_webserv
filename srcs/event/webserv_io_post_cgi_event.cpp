#include "webserv_io_post_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>


WebservIOPostCGIEvent::WebservIOPostCGIEvent()
{
    ;
}

WebservIOPostCGIEvent::~WebservIOPostCGIEvent()
{
    ;
}

WebservIOPostCGIEvent *WebservIOPostCGIEvent::singleton = NULL;
WebservIOPostCGIEvent *WebservIOPostCGIEvent::get_instance()
{
    if (WebservIOPostCGIEvent::singleton == NULL){
        singleton = new WebservIOPostCGIEvent();
    }
    return (singleton);
}

void WebservIOPostCGIEvent::delete_myself()
{
    if(WebservIOPostCGIEvent::singleton)
        delete WebservIOPostCGIEvent::singleton;
}



WebservEvent *WebservIOPostCGIEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservIOPostCGIEvent::from_fd");
    WebservIOPostCGIEvent *io_event = WebservIOPostCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work_reverse_io<WebservIOPostCGIEvent>, event->entity());
    return (new_event);
}

WebservEvent* WebservIOPostCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOPostCGIEvent::make_next_event() is_cgi_read:" + Utility::to_string(event->entity()->io().is_cgi_read()));

    if (event->entity()->request()->read_completed() && event->entity()->io().is_cgi_read() == false){
        return (event_factory->make_clean_event(event, false));
    }
    return (event_factory->make_waiting_post_cgi(event));
}

E_EpollEvent WebservIOPostCGIEvent::epoll_event(WebservEvent *event)
{
    DEBUG("WebservIOPostCGIEvent::epoll_event");
    (void)event;

    if (event->entity()->request()->read_completed() && event->entity()->io().is_cgi_read() == false){
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI_POST);
}

void WebservIOPostCGIEvent::check_completed(WebservEntity * entity)
{
    DEBUG("WebservIOPostCGIEvent::check_completed");
    if (entity->io().is_cgi_read()){
        WebservFile *file = entity->io().destination();

        size_t total_size = entity->io().total_write_size();
        size_t content_length = entity->request()->header().get_content_length();
        bool is_completed = file->completed();
        if((file->is_chunk() == false && content_length == total_size)){
            is_completed = true;
        }else if(file->is_chunk() == true  && is_completed){
            is_completed = true;
        }else{
            is_completed = false;
        }

        if(is_completed){
            char tmp[2] = {0};
            tmp[0] = EOF;
            char *tmp_p = tmp;
            int result = file->write(&tmp_p, 1);
            if(result <= 0){
                is_completed = false;
            }
        }
        entity->request()->set_read_completed(is_completed);
        entity->set_completed(true);
        return ;
    }else{
        // read from CGI, write to socket
        entity->set_completed(true);
        return ;
    }
}
