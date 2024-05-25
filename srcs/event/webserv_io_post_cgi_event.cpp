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


WebservEvent *WebservIOPostCGIEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservIOPostCGIEvent::from_fd");
    WebservIOPostCGIEvent *io_event = WebservIOPostCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work_reverse_io<WebservIOPostCGIEvent>, event->entity());
    return (new_event);
}

WebservEvent* WebservIOPostCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOPostCGIEvent::make_next_event()");

    if (event->entity()->completed() && event->entity()->io().is_cgi_read() == false){
        DEBUG("WebservIOPostCGIEvent::make_next_event() clean");
        return (event_factory->make_clean_event(event, false));
    }
    return (event_factory->make_waiting_post_cgi(event));
}

E_EpollEvent WebservIOPostCGIEvent::epoll_event(WebservEvent *event)
{
    DEBUG("WebservIOPostCGIEvent::epoll_event");
    (void)event;

    if (event->entity()->completed() && event->entity()->io().is_cgi_read() == false){
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
        DEBUG("WebservIOPostCGIEvent::check_completed No.1 completed:" + Utility::to_string(is_completed));
        //if(is_completed){
        //is_completed = true;

        if(is_completed){
        DEBUG("WebservIOPostCGIEvent::check_completed No.2 completed:" + Utility::to_string(is_completed));
            char tmp[2] = {0};
            tmp[0] = EOF;
            //tmp[0] = '\n';
            char *tmp_p = tmp;
            int result = file->write(&tmp_p, 1);
            if(result <= 0){
                is_completed = false;
            }
        }
        entity->set_completed(is_completed);
        return ;
    }else{
        DEBUG("WebservIOPostCGIEvent::check_completed No.2");
        // read from CGI, write to socket
        WebservFile *dst = entity->io().destination_for_read();
        bool flag = dst->completed();
        entity->set_completed(flag);
        return ;
    }
    return ;

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

            int wstatus;
            int result = waitpid(entity->app_result()->pid().to_int(), &wstatus,   WNOWAIT );

        if(result == -1){
            DEBUG("waitpid ERROR");
        }else{
        DEBUG("WebservIOPostCGIEvent::check_completed EPOLLOUT No.2");


            //flag = true;
        if(result == -1){
            DEBUG("waitpid ERROR");
        }else{
            if(WIFEXITED(wstatus)){
                DEBUG("exited, status=" + Utility::to_string(WEXITSTATUS(wstatus)));
            } else if (WIFSIGNALED(wstatus)) {
                DEBUG("killed by  status=" + Utility::to_string(WTERMSIG(wstatus)));
            } else if (WIFSTOPPED(wstatus)) {
                DEBUG("stopped by signal =" + WSTOPSIG(wstatus));
            } else if (WIFCONTINUED(wstatus)) {
                DEBUG("continued\n");

            }

        }
        }

    }

    DEBUG("WebservIOSocketEvent::check_completed end flag:" + Utility::to_string(flag));
    entity->set_completed(flag);
}
