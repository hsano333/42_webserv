#ifndef WEBSERV_READ_CGI_EVENT_HPP
#define WEBSERV_READ_CGI_EVENT_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_cgi_event.hpp"
#include "http_data.hpp"
#include "file.hpp"
#include "webserv_entity.hpp"

// source_fileからデータを最大8k-10バイト程度読み出し、これを保持する。
class WebservReadCGIEvent : public WebservEvent
{
    public:
        WebservReadCGIEvent();
        ~WebservReadCGIEvent();
        EWebservEvent which();

        static WebservReadCGIEvent *from_fd(FileDiscriptor fd, FileDiscriptor sockfd, IReader *reader);
        static WebservReadCGIEvent *from_cgi_fd(FileDiscriptor sockfd, IReader *reader);
        static WebservReadCGIEvent *from_event(WebservEvent *event, FileDiscriptor sockfd, IReader *reader);
        FileDiscriptor  &fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        void            switching_io(uint32_t epoll_event);
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
        void work();
        int read(char *buf, size_t size);

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();
        //IReader *reader();
        WebservEntity *entity();

    private:
        WebservReadCGIEvent(FileDiscriptor fd, IReader *reader);
        Request         *req_;
        Response        *res_;
        File            *source_file;
        File            *destination_file;
        FileDiscriptor fd_;
        //EWebservEvent event_type;
        int timeout_count_;
        bool is_completed_;
        //IWriter *writer;
        IReader *reader;
        WebservCgiEvent *cgi_event_;
        WebservEntity         *entity_;
};
#endif
