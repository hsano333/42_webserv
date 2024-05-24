#include "webserv_io_get_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>
#include <sys/types.h>
#include <sys/wait.h>


WebservIOGetCGIEvent::WebservIOGetCGIEvent()
{
    ;
}

WebservIOGetCGIEvent::~WebservIOGetCGIEvent()
{
    ;
}

WebservIOGetCGIEvent *WebservIOGetCGIEvent::singleton = NULL;
WebservIOGetCGIEvent *WebservIOGetCGIEvent::get_instance()
{
    if (WebservIOGetCGIEvent::singleton == NULL){
        singleton = new WebservIOGetCGIEvent();
    }
    return (singleton);
}


WebservEvent *WebservIOGetCGIEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservIOGetCGIEvent::from_fd");
    WebservIOGetCGIEvent *io_event = WebservIOGetCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work_reverse_io<WebservIOGetCGIEvent>, event->entity());
    return (new_event);
}

WebservEvent* WebservIOGetCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservIOGetCGIEvent::make_next_event()");

    WebservEvent* new_event = NULL;
    if (event->entity()->completed()){
        new_event = (event_factory->make_clean_event(event, false));
    }else{
        new_event = (event_factory->make_waiting_get_cgi(event));
    }

    return (new_event);
}

E_EpollEvent WebservIOGetCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    if (event->entity()->completed()){
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI_GET);
}

void WebservIOGetCGIEvent::check_completed(WebservEntity * entity)
{

    // copy from WebservIOSocketEvent
    size_t total_size = entity->io().destination()->size();
    DEBUG("WebservIOGetCGIEvent::check_completed size=" + Utility::to_string(total_size));

    bool flag = false;
    if(entity->io().in_out() == EPOLLIN){
        // CGI Scriptからの読み取りのみによって決定する。
        DEBUG("WebservIOCGIEvent::check_completed EPOLLIN");
        WebservFile *dst = entity->io().destination_for_read();
        flag = dst->completed();
    }else{ //EPOLL_OUT
        DEBUG("WebservIOCGIEvent::check_completed EPOLLOUT");
        flag = entity->response()->read_completed();
        /*
        int wstatus;
        int result = waitpid(entity->app_result()->pid().to_int(), &wstatus,   WNOWAIT );
        DEBUG("WebservIOCGIEvent::check_completed EPOLLOUT No.1 result=" + Utility::to_string(result));
        if(result == -1){
            DEBUG("waitpid ERROR");
        }else{
        DEBUG("WebservIOCGIEvent::check_completed EPOLLOUT No.2");
            flag = true;

        }
        */
    }

    MYINFO("WebservIOSocketEvent::check_completed end flag:" + Utility::to_string(flag));
    entity->set_completed(flag);
}
