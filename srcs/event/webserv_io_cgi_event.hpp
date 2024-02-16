#ifndef WEBSERV_IO_CGI_EVENT_HPP
#define WEBSERV_IO_CGI_EVENT_HPP
#include "webserv_event.hpp"
#include "webserv_io_event.hpp"
#include "file_discriptor.hpp"
#include "iwriter.hpp"
#include "ireader.hpp"
#include "file.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_entity.hpp"

// source_fileからデータを読み出し、fdに対して書き込む
class WebservIOCGIEvent
{
    public:
        ~WebservIOCGIEvent();
        static WebservEvent *from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, File *read_src, File *read_dst, File *write_src, File *write_dst, WebservEvent *event);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);
        static WebservIOCGIEvent *from_event(WebservEvent *event, File *io, File *write_src, File *read_dst);
    private:
        WebservIOCGIEvent();
        static WebservIOCGIEvent *singleton;
        static WebservIOCGIEvent *get_instance();
        WebservIOCGIEvent(FileDiscriptor  &fd, FileDiscriptor  &sockfd);
};
#endif
