/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_waiting_cgi_in_event.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 01:24:33 by hsano             #+#    #+#             */
/*   Updated: 2024/05/16 02:54:50 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv_waiting_cgi_in_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>


WebservWaitingCGIInEvent::WebservWaitingCGIInEvent()
{
    ;
}

WebservWaitingCGIInEvent::~WebservWaitingCGIInEvent()
{
    ;
}

WebservWaitingCGIInEvent *WebservWaitingCGIInEvent::singleton = NULL;
WebservWaitingCGIInEvent *WebservWaitingCGIInEvent::get_instance()
{
    if (WebservWaitingCGIInEvent::singleton == NULL){
        singleton = new WebservWaitingCGIInEvent();
    }
    return (singleton);
}

WebservEvent *WebservWaitingCGIInEvent::from_event(WebservEvent * event)
{
    DEBUG("WebservWaitingCGIInEvent::from_fd");
    WebservWaitingCGIInEvent *io_event = WebservWaitingCGIInEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, io_work<WebservWaitingCGIInEvent>, event->entity());
    //new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}


WebservEvent *WebservWaitingCGIInEvent::from_fd(FileDiscriptor &read_fd, FileDiscriptor &write_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    DEBUG("WebservWaitingCGIInEvent::from_fd");
    WebservWaitingCGIInEvent *io_event = WebservWaitingCGIInEvent::get_instance();
    WebservEvent *new_event = new WebservEvent( io_event, io_work<WebservWaitingCGIInEvent>, event->entity());

    /*
    new_event->entity()->io().set_read_io(read_src, read_dst);
    new_event->entity()->io().set_write_io(write_src, write_dst);

    new_event->entity()->io().set_read_fd(read_fd);
    new_event->entity()->io().set_write_fd(write_fd);
    */
    new_event->entity()->io().set_read_io(write_src, write_dst);
    new_event->entity()->io().set_write_io(read_src, read_dst);

    new_event->entity()->io().set_read_fd(write_fd);
    new_event->entity()->io().set_write_fd(read_fd);
    if(write_src == NULL){
        DEBUG("write_src is NULL");
    }
    if(write_dst == NULL){
        DEBUG("write_dst is NULL");
    }
    if(read_src == NULL){
        DEBUG("read_src is NULL");
    }
    if(read_dst == NULL){
        DEBUG("read_dst is NULL");
    }
    //new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}

WebservEvent* WebservWaitingCGIInEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservWaitingCGIInEvent::make_next_event()");
    /*
    if(event->entity()->io().cgi_divided() == false){
        DEBUG("WebservWaitingCGIInEvent::make_next_event() divided");
        event->entity()->io().set_cgi_divided(true);
        event->entity()->set_completed(false);
        return (event_factory->make_waiting_out_cgi(event));
    }
    */

    /*
    if(event->entity()->response() == NULL){
        DEBUG("WebservWaitingCGIInEvent::make_next_event() No.1");
        return (event_factory->make_making_response_for_cgi_event(event));
    }
    */

    if(event->entity()->response() == NULL && event->entity()->io().is_cgi_read() == false){
        DEBUG("WebservWaitingCGIInEvent::make_next_event() No.1");
        return (event_factory->make_making_response_for_post_cgi_event(event));
    }

    DEBUG("WebservWaitingCGIInEvent::make_next_event() No.2 read:" + Utility::to_string(event->entity()->io().is_cgi_read()));
    return (event_factory->make_io_socket_for_post_cgi(event));

    /*
    if(event->entity()->io().in_out() == EPOLLIN){
        DEBUG("WebservWaitingCGIInEvent::make_next_event() No.1");
        return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));
    }
    return (event);
    */
}

E_EpollEvent WebservWaitingCGIInEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservWaitingCGIInEvent::epoll_event()");
    if(event->entity()->response() == NULL && event->entity()->io().is_cgi_read() == false){
        return (EPOLL_NONE);

    }

    /*
    if(event->entity()->io().cgi_divided() == true && event->entity()->completed() == false){
        DEBUG("WebservWaitingCGIInEvent::epoll_event() DIvided");
        return (EPOLL_NONE);
    }
    */

    //CGI WRITE
    //
    if(event->entity()->io().is_cgi_read()){

        event->entity()->io().set_is_cgi_read(true);
        // EPOLLOUT is CGI_OUT(write cgi), so next is waiting socket in.
        DEBUG("WebservWaitingCGIInEvent::epoll_event() No.2");
        //end Socket in
        //event->entity()->io().switching_io(EPOLLIN);

        // 入力がまだ残っている時はEPOLL_READ,そうでないときはEPOLL_NONE
        // todo
        //return (EPOLL_READ);
        return (EPOLL_NONE);
    }else{
        event->entity()->io().set_is_cgi_read(false);
        // EPOLLIN is CGI_IN(read cgi), so next is waiting socket out.
        DEBUG("WebservWaitingCGIInEvent::epoll_event() No.3");
        //event->entity()->io().switching_io(EPOLLOUT);
        //end Socket in
        return (EPOLL_WRITE);

    }

    // next event
        //DEBUG("WebservWaitingCGIInEvent::epoll_event() No.3");
    //return (EPOLL_FOR_CGI_OUT);
    /*
    if (event->entity()->completed()){
        DEBUG("WebservWaitingCGIInEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI);


    DEBUG("WebservWaitingCGIInEvent::epoll_event()");
    //return (EPOLL_FOR_CGI);
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            DEBUG("WebservWaitingCGIInEvent::epoll_event() No.1");
            return (EPOLL_NONE);
        }else{
            DEBUG("WebservWaitingCGIInEvent::epoll_event() No.2");
            return (EPOLL_READ);
        }
    }
            DEBUG("WebservWaitingCGIInEvent::epoll_event() No.3");
    return (EPOLL_NONE);
    */
}

void WebservWaitingCGIInEvent::check_completed(WebservEntity * entity)
{
    DEBUG("WebservWaitingCGIInEvent::check_completed No.1");
    entity->set_completed(true);
    /*
    return ;

    if(entity->io().cgi_divided() == false){
    DEBUG("WebservWaitingCGIInEvent::check_completed No.2");
        event->entity()->io().set_is_cgi_read(true);
        return ;
    }
    */
    // EPOLLOUTの時、CGIからの出力
    // EPOLLINの時、SOCKETに対する入力（クライアントに対する出力)
    if(entity->io().in_out() == EPOLLIN){
        entity->set_completed(true);
        DEBUG("WebservWaitingCGIInEvent::check_completed No.3");
        entity->io().set_is_cgi_read(false);
        /*
        //entity->set_completed(true);
        WebservFile *file = entity->io().destination();
        //WebservFile *file = entity->io().source_for_read();

        size_t total_size = entity->io().total_write_size();
        size_t content_length = entity->request()->header().get_content_length();
        bool is_completed = file->completed();
        if(content_length == total_size && is_completed){
            is_completed = true;
        }else{
            is_completed = false;
        }
        DEBUG("WebservApplicationUploadEvent::check_completed :" + Utility::to_string(is_completed));
        DEBUG("WebservApplicationUploadEvent::total_size :" + Utility::to_string(total_size));
        entity->set_completed(is_completed);

        if(is_completed){
            char *tmp = NULL;
                // 一度目だけステータスコードを読み取れる。
                // ２回目以降は読み取れなくなる
            int status_code = file->read(&tmp, 0);
            entity->app_result()->set_status_code(status_code);
        }
        */
    }else{
        entity->io().set_is_cgi_read(true);

    }
    DEBUG("WebservWaitingCGIInEvent::check_completed No.4");
}
