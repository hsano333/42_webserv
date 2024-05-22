#include "webserv_io_post_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
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


WebservEvent *WebservIOPostCGIEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservIOPostCGIEvent::from_fd");
    WebservIOPostCGIEvent *io_event = WebservIOPostCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work<WebservIOPostCGIEvent>, event->entity());
    return (new_event);
}

WebservEvent* WebservIOPostCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOPostCGIEvent::make_next_event()");

    if (event->entity()->completed() && event->entity()->io().is_cgi_read() == false){
        return (event_factory->make_clean_event(event, false));
    }
    return (event_factory->make_waiting_cgi(event));
}

E_EpollEvent WebservIOPostCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;

    if (event->entity()->completed() && event->entity()->io().is_cgi_read() == false){
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI_POST);
}

void WebservIOPostCGIEvent::check_completed(WebservEntity * entity)
{
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
        //if(is_completed){
        is_completed = true;
            entity->set_completed(is_completed);
            char tmp[2] = {0};
            tmp[0] = EOF;
            char *tmp_p = tmp;
            file->write(&tmp_p, 1);
        //}
        return ;

    }else{
        // read from CGI, write to socket
        entity->set_completed(true);
        return ;
    }

    //todo 
    //
    //entity->set_completed(true);
    //return 
    //todo
    DEBUG("WebservIOPostCGIEvent::check_completed");
    //entity->set_completed(true);
    //return ;

    // copy from WebservIOSocketEvent
    size_t total_size = entity->io().destination()->size();
    DEBUG("WebservIOPostCGIEvent::check_completed size=" + Utility::to_string(total_size));

    bool flag = false;
    if(entity->io().in_out() == EPOLLIN){
        // CGI Scriptからの読み取りのみによって決定する。
        DEBUG("WebservIOSocketEvent::check_completed EPOLLIN");
        WebservFile *dst = entity->io().destination_for_read();
        flag = dst->completed();
    }else{ //EPOLL_OUT
        DEBUG("WebservIOSocketEvent::check_completed EPOLLOUT");
        flag = entity->response()->read_completed();
    }

    DEBUG("WebservIOSocketEvent::check_completed end flag:" + Utility::to_string(flag));
    entity->set_completed(flag);
}
