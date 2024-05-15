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

/*
WebservEvent *WebservIOPostCGIEvent::from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    DEBUG("WebservIOPostCGIEvent::from_fd");
    WebservIOPostCGIEvent *io_event = WebservIOPostCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work<WebservIOPostCGIEvent>, event->entity());
    new_event->entity()->io().set_read_io(read_src, read_dst);
    new_event->entity()->io().set_write_io(write_src, write_dst);

    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().set_write_fd(write_fd);
    new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}
*/

WebservEvent* WebservIOPostCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOPostCGIEvent::make_next_event()");

    //WebservEvent* new_event = NULL;
    //if (event->entity()->completed() && EPOLLOUT){
        //new_event = (event_factory->make_clean_event(event, false));
    //}else{
        //new_event = (event_factory->make_waiting_out_cgi(event));
    //}

    if (event->entity()->completed() && event->entity()->io().is_cgi_read() == false){
        DEBUG("WebservIOPostCGIEvent::make_next_event() No.1");
        return (event_factory->make_clean_event(event, false));
    }
        DEBUG("WebservIOPostCGIEvent::make_next_event() No.2");
    //return (new_event);
    return (event_factory->make_waiting_cgi(event));
}

E_EpollEvent WebservIOPostCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;

    DEBUG("WebservIOPostCGIEvent::epoll_event()");
    //if (event->entity()->completed() && event->entity()->io().in_out() == EPOLLOUT){
    if (event->entity()->completed() && event->entity()->io().is_cgi_read() == false){
        DEBUG("WebservIOPostCGIEvent::epoll_event() end");
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI_POST);
    //return (EPOLL_WRITE);

    if(event->entity()->io().in_out() == EPOLLIN){
        return (EPOLL_WRITE);
    }else{
        return (EPOLL_READ);
    }

    return (EPOLL_NONE);
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
        DEBUG("WebservIOPostCGIEvent::check_completed:" + Utility::to_string(is_completed));
        DEBUG("WebservIOPostCGIEvent::check_completed::total_size :" + Utility::to_string(total_size));
        DEBUG("WebservIOPostCGIEvent::check_completed::content_length :" + Utility::to_string(content_length));
        DEBUG("WebservIOPostCGIEvent::check_completed::is_completed :" + Utility::to_string(is_completed));
        entity->set_completed(is_completed);
        //char *nl = &NL_CGI;
        //char const *tmp = NL2_CGI;
        char nl = NL2_CGI[0];
        char *tmp = &nl;
        file->write(&(tmp), 1);
        return ;
        // read from socket, write to cgi

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
           //
        flag = entity->response()->read_completed();
        //WebservFile *src = entity->io().source_for_write();

        /*
        if(entity->response()->header().is_chunked()){

        }else{
            DEBUG("WebservIOSocketEvent::check_completed EPOLLOUT write_size:" + Utility::to_string(src->size()));
            DEBUG("WebservIOSocketEvent::check_completed EPOLLOUT get_content_length():" + Utility::to_string(entity->response()->header().get_content_length()));
            if(src->size() >= (size_t)entity->request()->header().get_content_length()){
                DEBUG("WebservIOSocketEvent::check_completed EPOLLOUT close()");
                src->close();
            }
        }
        */
        //flag = src->completed();
    }

    DEBUG("WebservIOSocketEvent::check_completed end flag:" + Utility::to_string(flag));
    entity->set_completed(flag);
}
