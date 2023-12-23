#ifndef WEBSERV_EVENT_PARSER_HPP
#define WEBSERV_EVENT_PARSER_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"
#include "webserv_cgi_event.hpp"

class WebservParserEvent : public WebservEvent
{
    public:
        WebservParserEvent(FileDiscriptor fd, Request *req);
        ~WebservParserEvent();
        static WebservParserEvent *from_event(WebservEvent *event);
        EWebservEvent   which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        FileDiscriptor  fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();

        bool is_completed();
        void set_completed(bool flag);

        void increase_timeout_count(int count);
        int  timeout_count();
        //void            set_response(Response *res);
        //File *file();
        //void set_file(File *file);

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();

    private:
        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        File            *file_;
        File            *source_file;
        File            *destination_file;

        int             timeout_count_;
        bool            is_completed_;

        WebservCgiEvent *cgi_event_;
};

#endif
