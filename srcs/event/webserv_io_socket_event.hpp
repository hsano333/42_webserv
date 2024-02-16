#ifndef WEBSERV_IO_SOCKET_EVENT_HPP
#define WEBSERV_IO_SOCKET_EVENT_HPP
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
class WebservIOSocketEvent
{
    public:
        WebservIOSocketEvent(File *socket_io, File *read_dst);
        ~WebservIOSocketEvent();
        static WebservIOSocketEvent *for_cgi(File *src, File *read_dst);
        static WebservEvent *as_read(FileDiscriptor const &read_fd, File *src, File *dst, WebservEntity *entity);
        static WebservEvent *as_write(WebservEvent *event, FileDiscriptor const &write_fd, File *src, File *dst);

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
