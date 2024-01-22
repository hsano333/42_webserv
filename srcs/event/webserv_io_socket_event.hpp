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

// source_fileからデータを読み出し、fdに対して書き込む
class WebservIOSocketEvent : public WebservEvent, public WebservIOEvent
{
    public:
        WebservIOSocketEvent(FileDiscriptor &fd, FileDiscriptor sockfd, File *socket_io, File *read_dst);
        ~WebservIOSocketEvent();
        static WebservIOSocketEvent *for_cgi(FileDiscriptor &fd, FileDiscriptor &sockfd, File *src, File *read_dst);
        static WebservIOSocketEvent *as_read(FileDiscriptor &fd, FileDiscriptor &sockfd, File *src, File *dst);
        static WebservIOSocketEvent *as_write(FileDiscriptor &fd, FileDiscriptor &sockfd, File *src, File *dst);


        EWebservEvent which();

        FileDiscriptor  &fd();
        FileDiscriptor  &sock_fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        void            switching_io(uint32_t epoll_event);
        void            set_src(File *file);
        void            set_dst(File *file);
        void            set_write_io(File *src, File *dst);
        void            set_read_io(File *src, File *dst);
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        int write(char *buf, size_t size);
        int read(char *buf, size_t size);
        //static WebservIOSocketEvent *from_event(WebservEvent *event, File *src, File *dst);

    private:
        WebservIOSocketEvent();
        WebservIOSocketEvent(FileDiscriptor  &fd, FileDiscriptor  &sockfd);
        FileDiscriptor  fd_;
        FileDiscriptor  sock_fd_;
        Request         *req_;
        Response        *res_;
        File            *io;
        File            *read_src;
        File            *read_dst;
        File            *write_src;
        File            *write_dst;
        File            *source_file;
        File            *destination_file;
        int             timeout_count_;
        bool is_completed_;
        WebservCgiEvent *cgi_event_;
        uint32_t        in_out;
};
#endif
