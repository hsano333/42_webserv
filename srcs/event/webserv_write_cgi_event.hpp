#ifndef WEBSERV_EVENT_CGI_WRITE_HPP
#define WEBSERV_EVENT_CGI_WRITE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "iwriter.hpp"
#include "response.hpp"
#include "file.hpp"
#include "webserv_cgi_event.hpp"
#include "http_data.hpp"
#include "application_result.hpp"

// source_fileからデータを読み出し、fdに対して書き込む
class WebservWriteCGIEvent : public WebservEvent
{
    public:
        WebservWriteCGIEvent();
        WebservWriteCGIEvent(FileDiscriptor fd);
        WebservWriteCGIEvent(FileDiscriptor fd, Request *req, Response *res, File *source, IWriter* writer);
        ~WebservWriteCGIEvent();
        EWebservEvent which();

        FileDiscriptor  &fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        void            switching_io(uint32_t epoll_event);
        void            set_src(File *file);
        void            set_dst(File *file);
        //HttpData        *source();
        //File            *source();
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        int write(char *buf, size_t size);
        static WebservWriteCGIEvent *from_error_status_code(WebservEvent *event, StatusCode &code, File *file, IWriter *writer);
        static WebservWriteCGIEvent *from_event_for_cgi(WebservEvent *event, Response *res, IWriter *writer);
        static WebservWriteCGIEvent *from_event(WebservEvent *event, File *src, File *dst, ApplicationResult *result);
        static WebservWriteCGIEvent *from_cgi_fd(FileDiscriptor fd, Request *req, IReader *reader, IWriter *writer);
        //void set_cgi_event(WebservCgiEvent *cgi_event);
        //WebservCgiEvent *cgi_event();

    private:
        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        //HttpData        *source_;
        File            *source_file;
        File            *destination_file;
        ApplicationResult *result;
        int             timeout_count_;
        //IWriter *writer;
        //IReader *reader;
        bool is_completed_;
        WebservCgiEvent *cgi_event_;
};
#endif
