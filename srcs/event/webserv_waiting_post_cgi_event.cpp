/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_waiting_post_cgi_event.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 01:24:33 by hsano             #+#    #+#             */
/*   Updated: 2024/05/25 03:00:13 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv_waiting_post_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include <assert.h>


WebservWaitingPostCGIEvent::WebservWaitingPostCGIEvent()
{
    ;
}

WebservWaitingPostCGIEvent::~WebservWaitingPostCGIEvent()
{
    ;
}

WebservWaitingPostCGIEvent *WebservWaitingPostCGIEvent::singleton = NULL;
WebservWaitingPostCGIEvent *WebservWaitingPostCGIEvent::get_instance()
{
    if (WebservWaitingPostCGIEvent::singleton == NULL){
        singleton = new WebservWaitingPostCGIEvent();
    }
    return (singleton);
}

namespace myfunc{
    bool check_completed(WebservWaitingPostCGIEvent *event, WebservEntity *entity)
    {
        event->check_completed(entity);
        return (true);
    }
}


WebservEvent *WebservWaitingPostCGIEvent::from_event(WebservEvent * event)
{
    DEBUG("WebservWaitingCGIInEvent::from_fd");
    WebservWaitingPostCGIEvent *io_event = WebservWaitingPostCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, myfunc::check_completed, event->entity());
    return (new_event);
}

/*
WebservEvent *WebservWaitingPostCGIEvent::from_event(WebservEvent * event)
{
    DEBUG("WebservWaitingPostCGIEvent::from_fd");
    WebservWaitingPostCGIEvent *io_event = WebservWaitingPostCGIEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( io_event, dummy_func<WebservWaitingPostCGIEvent>, event->entity());
    //new_event->entity()->io().switching_io(EPOLLIN);
    return (new_event);
}
*/


/*
WebservEvent *WebservWaitingPostCGIEvent::from_fd(FileDiscriptor &read_fd, FileDiscriptor &write_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent * event)
{
    DEBUG("WebservWaitingPostCGIEvent::from_fd");
    WebservWaitingPostCGIEvent *io_event = WebservWaitingPostCGIEvent::get_instance();
    WebservEvent *new_event = new WebservEvent( io_event, io_work<WebservWaitingPostCGIEvent>, event->entity());

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
*/

WebservEvent* WebservWaitingPostCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservWaitingPostCGIEvent::make_next_event()");
    /*
    if(event->entity()->io().cgi_divided() == false){
        DEBUG("WebservWaitingPostCGIEvent::make_next_event() divided");
        event->entity()->io().set_cgi_divided(true);
        event->entity()->set_completed(false);
        return (event_factory->make_waiting_out_cgi(event));
    }
    */

    /*
    if(event->entity()->response() == NULL){
        DEBUG("WebservWaitingPostCGIEvent::make_next_event() No.1");
        return (event_factory->make_making_response_for_cgi_event(event));
    }
    */

    if(event->entity()->response() == NULL && event->entity()->io().is_cgi_read() == false){
        DEBUG("WebservWaitingPostCGIEvent::make_next_event() No.1");
        return (event_factory->make_making_response_for_post_cgi_event(event));
    }

    DEBUG("WebservWaitingPostCGIEvent::make_next_event() No.2 read:" + Utility::to_string(event->entity()->io().is_cgi_read()));
    return (event_factory->make_io_socket_for_post_cgi(event));

    /*
    if(event->entity()->io().in_out() == EPOLLIN){
        DEBUG("WebservWaitingPostCGIEvent::make_next_event() No.1");
        return (event_factory->make_making_response_event(event, event->entity()->io().destination_for_read()));
    }
    return (event);
    */
}

E_EpollEvent WebservWaitingPostCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservWaitingPostCGIEvent::epoll_event()");
    if(event->entity()->response() == NULL && event->entity()->io().is_cgi_read() == false){
        return (EPOLL_NONE);
    }

    if(event->entity()->io().is_cgi_read()){
        DEBUG("WebservWaitingPostCGIEvent::epoll_event() READ");
        WebservFile *file = event->entity()->io().source_for_write();

        if(file->can_read()){
            return (EPOLL_NONE);
        }
        return (EPOLL_READ);
    }else{
        DEBUG("WebservWaitingPostCGIEvent::epoll_event() WRITE");
        //WebservFile *file = event->entity()->io().destination_for_write();
        return (EPOLL_WRITE);
    }

    /*
    if(event->entity()->io().is_cgi_read()){
        event->entity()->io().set_is_cgi_read(true);
    }else{
        event->entity()->io().set_is_cgi_read(false);
    }
    */


    return (EPOLL_NONE);
    /*
    if(event->entity()->io().cgi_divided() == true && event->entity()->completed() == false){
        DEBUG("WebservWaitingPostCGIEvent::epoll_event() DIvided");
        return (EPOLL_NONE);
    }
    */

    //CGI WRITE
    //
    if(event->entity()->io().is_cgi_read()){

        //event->entity()->io().set_is_cgi_read(true);
        // EPOLLOUT is CGI_OUT(write cgi), so next is waiting socket in.
        DEBUG("WebservWaitingPostCGIEvent::epoll_event() No.2");
        //end Socket in
        //event->entity()->io().switching_io(EPOLLIN);

        // 入力がまだ残っている時はEPOLL_READ,そうでないときはEPOLL_NONE
        // todo
        //return (EPOLL_READ);
        //return (EPOLL_NONE);
    }else{
        //event->entity()->io().set_is_cgi_read(false);
        // EPOLLIN is CGI_IN(read cgi), so next is waiting socket out.
        DEBUG("WebservWaitingPostCGIEvent::epoll_event() No.3");
    }

    // next event
        //DEBUG("WebservWaitingPostCGIEvent::epoll_event() No.3");
    //return (EPOLL_FOR_CGI_OUT);
    /*
    if (event->entity()->completed()){
        DEBUG("WebservWaitingPostCGIEvent::epoll_event() No.1");
        return (EPOLL_NONE);
    }
    return (EPOLL_FOR_CGI);


    DEBUG("WebservWaitingPostCGIEvent::epoll_event()");
    //return (EPOLL_FOR_CGI);
    if(event->entity()->io().in_out() == EPOLLIN){
        if (event->entity()->completed()){
            DEBUG("WebservWaitingPostCGIEvent::epoll_event() No.1");
            return (EPOLL_NONE);
        }else{
            DEBUG("WebservWaitingPostCGIEvent::epoll_event() No.2");
            return (EPOLL_READ);
        }
    }
            DEBUG("WebservWaitingPostCGIEvent::epoll_event() No.3");
    return (EPOLL_NONE);
    */
}

void WebservWaitingPostCGIEvent::check_completed(WebservEntity * entity)
{
    DEBUG("WebservWaitingPostCGIEvent::check_completed No.1");
    entity->set_completed(true);

    // EPOLLOUTの時、CGIからの出力
    // EPOLLINの時、SOCKETに対する入力（クライアントに対する出力)
    if(entity->io().in_out() == EPOLLOUT){
        //entity->set_completed(true);
        DEBUG("WebservWaitingPostCGIEvent::check_completed No.3");
        entity->io().set_is_cgi_read(true);
    }else{
        entity->io().set_is_cgi_read(false);

    }
    DEBUG("WebservWaitingPostCGIEvent::check_completed No.4");
}
