#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP
#include "epoll.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_cgi_event.hpp"

typedef enum E_WebservEvent
{
    INIT_EVENT = 0,
    READ_EVENT,
    IO_EVENT,
    //PARSER_EVENT,
    APPLICATION_EVENT,
    WRITE_EVENT,
    CLEAN_EVENT,
    TIMEOUT_EVENT,
    KEEPA_ALIVE_EVENT,
    NOTHING_EVENT,
    //CGI_EVENT,
    MAKE_EVENT,
} EWebservEvent;

typedef enum E_EpollEvent
{
    EPOLL_READ,
    EPOLL_WRITE,
    EPOLL_CONTINUE,
    EPOLL_CLOSE,
    EPOLL_CGI_IN,
    EPOLL_CGI_OUT,
    EPOLL_CGI_STOP,
    EPOLL_NONE,
} E_EpollEvent;


class WebservEvent
{
    public:
        //WebservEvent();
        virtual ~WebservEvent(){};
        virtual EWebservEvent which() = 0;
        //static WebservEvent *from_epoll_event(t_epoll_event const &event);

        virtual FileDiscriptor fd() = 0;
        virtual WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory) = 0;
        virtual E_EpollEvent get_next_epoll_event() = 0;

        virtual Request *req() = 0;
        virtual Response *res() = 0;
        virtual File *src() = 0;
        virtual File *dst() = 0;
        virtual void set_dst(File *file) = 0;

        virtual bool is_completed() = 0;
        virtual void set_completed(bool flag) = 0;

        virtual void increase_timeout_count(int count) = 0;
        virtual int  timeout_count() = 0;

        virtual void set_cgi_event(WebservCgiEvent *cgi_event) = 0;
        virtual WebservCgiEvent *cgi_event() = 0;

    private:
        Request *req_;
        Response *res_;
        int timeout_count_;
};

#endif
