#ifndef WEBSERV_READ_EVENT_HPP
#define WEBSERV_READ_EVENT_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_cgi_event.hpp"
#include "http_data.hpp"
#include "file.hpp"
#include "webserv_entity.hpp"

// source_fileからデータを最大8k-10バイト程度読み出し、これを保持する。
class WebservReadEvent : public WebservEvent
{
    public:
        //WebservReadEvent();
        //WebservReadEvent(FileDiscriptor fd, IReader* ireader);
        ~WebservReadEvent();
        EWebservEvent which();

        //static WebservReadEvent *from_fd(FileDiscriptor fd, IReader *reader);
        static WebservReadEvent *from_fd(FileDiscriptor fd, FileDiscriptor sockfd, IReader *reader, File *src, File *dst);
        //static WebservReadEvent *from_cgi_fd(FileDiscriptor sockfd,FileDiscriptor pid, IReader *reader);
        static WebservReadEvent *from_event(WebservEvent *event, IReader *reader);
        //FileDiscriptor  &fd();
        //FileDiscriptor  &sock_fd();
        //Request         *req();
        //Response        *res();
        File            *src();
        File            *dst();
        void            set_src(File *file);
        void            set_dst(File *file);
        //File            *source();
        //HttpData *source();
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        //int read(char *buf, size_t size);

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();
        //IReader *reader();
        WebservEntity *entity();

    private:
        WebservReadEvent();
        //WebservReadEvent(FileDiscriptor fd, FileDiscriptor sockfd, IReader *reader);
        //Request         *req_;
        //Response        *res_;
        File            *source_file;
        File            *destination_file;
        //FileDiscriptor  fd_;
        //FileDiscriptor  sockfd_;
        //EWebservEvent event_type;
        int timeout_count_;
        bool is_completed_;
        //IWriter *writer;
        IReader *reader;
        WebservCgiEvent *cgi_event_;



        //int     raw_buf_space();
        //char*   get_raw_buf_pointer();
        //size_t  buf_size();
        //void    set_read_completed(bool flag);
        //void    set_buf_pos(size_t pos);
        char    buf_[MAX_BUF];
        char*   buf();
        size_t  raw_buf_pos_;
        WebservEntity         *entity_;
};
#endif
