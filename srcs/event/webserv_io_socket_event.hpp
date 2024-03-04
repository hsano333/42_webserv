#ifndef WEBSERV_IO_SOCKET_EVENT_HPP
#define WEBSERV_IO_SOCKET_EVENT_HPP
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
class WebservIOSocketEvent
{
    public:
        WebservIOSocketEvent(WebservFile *socket_io, WebservFile *read_dst);
        ~WebservIOSocketEvent();
        static WebservIOSocketEvent *for_cgi(WebservFile *src, WebservFile *read_dst);
        static WebservEvent *as_read(FileDiscriptor const &read_fd, WebservFile *src, WebservFile *dst, WebservEntity *entity);
        static WebservEvent *as_write(WebservEvent *event, FileDiscriptor const &write_fd, WebservFile *src, WebservFile *dst);

        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);
        int write(char *buf, size_t size);
        int read(char *buf, size_t size);

    private:
        WebservIOSocketEvent();
        static WebservIOSocketEvent *singleton;
        static WebservIOSocketEvent *get_instance();
};
#endif
