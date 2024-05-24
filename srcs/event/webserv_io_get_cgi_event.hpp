#ifndef WEBSERV_IO_CGI_EVENT_HPP
#define WEBSERV_IO_CGI_EVENT_HPP
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
class WebservIOGetCGIEvent
{
    public:
        ~WebservIOGetCGIEvent();
        //static WebservEvent *from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent *event);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);

        static WebservEvent *from_event(WebservEvent *event);
    private:
        WebservIOGetCGIEvent();
        static WebservIOGetCGIEvent *singleton;
        static WebservIOGetCGIEvent *get_instance();
        WebservIOGetCGIEvent(FileDiscriptor  &fd, FileDiscriptor  &sockfd);
};
#endif
