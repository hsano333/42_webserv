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
        static WebservIOSocketEvent *from_fd(FileDiscriptor &fd, FileDiscriptor &sockfd, File *socket_io, File *read_dst);


        EWebservEvent which();

        FileDiscriptor  fd();
        FileDiscriptor  sock_fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        void            set_io(uint32_t epoll_event);
        void            set_src(File *file);
        void            set_dst(File *file);
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
        FileDiscriptor  fd_;
        FileDiscriptor  sock_fd_;
        Request         *req_;
        Response        *res_;
        File            *io;
        File            *read_dst;
        File            *write_src;
        File            *source_file;
        File            *destination_file;
        int             timeout_count_;
        bool is_completed_;
        WebservCgiEvent *cgi_event_;
};
#endif
