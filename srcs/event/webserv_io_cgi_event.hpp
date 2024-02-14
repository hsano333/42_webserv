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
        //static WebservIOCGIEvent *from_fd(FileDiscriptor &fd, FileDiscriptor &sockfd, File *io, File *read_src, File *write_dst);
        static WebservEvent *from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, File *read_src, File *read_dst, File *write_src, File *write_dst, WebservEvent *event);
        //static WebservEvent *as_read(FileDiscriptor const &fd, FileDiscriptor &sockfd, File *src, File *dst);
        //static WebservEvent *as_write(FileDiscriptor const &fd, FileDiscriptor &sockfd, File *src, File *dst);


        EWebservEvent which();

        //FileDiscriptor  &fd();
        //FileDiscriptor  &sock_fd();
        //Request         *req();
        //Response        *res();
        //File            *src();
        //File            *dst();
        //void            switching_io(uint32_t epoll_event);
        //void            set_src(File *file);
        //void            set_dst(File *file);
        //void            set_write_io(File *src, File *dst);
        //void            set_read_io(File *src, File *dst);
        //FileDiscriptor  &get_write_fd();
        //FileDiscriptor  &get_read_fd();
        //FileDiscriptor  const &get_socket_fd();
        //bool is_completed();
        //void set_completed(bool flag);
        //void increase_timeout_count(int count);
        //int  timeout_count();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);
        //int write(char *buf, size_t size);
        //int read(char *buf, size_t size);

        static WebservIOCGIEvent *from_event(WebservEvent *event, File *io, File *write_src, File *read_dst);
        //WebservEntity *entity();

    private:
        WebservIOCGIEvent();
        static WebservIOCGIEvent *singleton;
        static WebservIOCGIEvent *get_instance();
        WebservIOCGIEvent(FileDiscriptor  &fd, FileDiscriptor  &sockfd);
        //FileDiscriptor  fd_;
        //FileDiscriptor  sock_fd_;
        //Request         *req_;
        //Response        *res_;
        //File            *io;
        //File            *read_src;
        //File            *read_dst;
        //File            *write_src;
        //File            *write_dst;
        //File            *source_file;
        //File            *destination_file;
        //int             timeout_count_;
        //bool is_completed_;
        //WebservCgiEvent *cgi_event_;
        //uint32_t        in_out;

        //FileDiscriptor  write_fd_;
        //FileDiscriptor  read_fd_;
        //WebservEntity         *entity_;
};
#endif
