
#ifndef WEBSERV_EVENT_WRITE_HPP
#define WEBSERV_EVENT_WRITE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "iwriter.hpp"
#include "response.hpp"
#include "file.hpp"
#include "webserv_cgi_event.hpp"

class WebservWriteEvent : public WebservEvent
{
    public:
        WebservWriteEvent();
        WebservWriteEvent(FileDiscriptor fd);
        WebservWriteEvent(FileDiscriptor fd, Request *req, Response *res, IWriter* writer);
        ~WebservWriteEvent();
        EWebservEvent which();

        FileDiscriptor  fd();
        Request         *req();
        Response        *res();
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        int write(char const *buf, size_t size);
        static WebservWriteEvent *from_error_status_code(WebservEvent *event, StatusCode &code, File *file, IWriter *writer);
        static WebservWriteEvent *from_event(WebservEvent *event, Response *res, IWriter *writer);
        static WebservWriteEvent *from_cgi_fd(FileDiscriptor fd, Request *req, IWriter *writer);

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();


    private:
        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        int             timeout_count_;
        IWriter *writer;
        bool is_completed_;
        WebservCgiEvent *cgi_event_;
        File            *source_file;
};
#endif
