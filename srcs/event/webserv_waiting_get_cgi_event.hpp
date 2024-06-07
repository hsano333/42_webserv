#ifndef WEBSERV_WAITINGl_CGI_OUT_EVENT_HPP
#define WEBSERV_WAITINGl_CGI_OUT_EVENT_HPP
#include "webserv_event.hpp"
#include "webserv_io_event.hpp"
#include "file_discriptor.hpp"
#include "iwriter.hpp"
#include "ireader.hpp"
#include "webserv_file.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_entity.hpp"

// source_fileからデータを読み出し、fdに対して書き込む
class WebservWaitingGetCGIEvent
{
    public:
        ~WebservWaitingGetCGIEvent();
        static WebservEvent *from_event(WebservEvent *event);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);
        static void delete_myself();

    private:
        WebservWaitingGetCGIEvent();
        static WebservWaitingGetCGIEvent *singleton;
        static WebservWaitingGetCGIEvent *get_instance();
        WebservWaitingGetCGIEvent(FileDiscriptor  &fd, FileDiscriptor  &sockfd);
};
#endif
