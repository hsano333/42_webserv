#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP
#include "epoll.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_event_factory.hpp"
//#include "webserv_cgi_event.hpp"

typedef enum E_WebservEvent
{
    INIT_EVENT = 0,
    READ_EVENT,
    IO_EVENT,
    APPLICATION_EVENT,
    WRITE_EVENT,
    CLEAN_EVENT, //5
    TIMEOUT_EVENT,
    KEEPA_ALIVE_EVENT,
    NOTHING_EVENT,
    MAKE_EVENT, //9
} EWebservEvent;

typedef enum E_EpollEvent
{
    EPOLL_READ,
    EPOLL_WRITE,
    EPOLL_ADD_READ,
    EPOLL_ADD_WRITE,
    EPOLL_FOR_CGI,
    EPOLL_CONTINUE,
    EPOLL_CLOSE,
    EPOLL_CGI_IN,
    EPOLL_CGI_OUT,
    EPOLL_CGI_STOP,
    EPOLL_CGI,
    EPOLL_NONE,
} E_EpollEvent;

/*
struct Entity
{
    Request             *req;
    Response            *res;
    ApplicationResult   *app;
};
*/

struct Entity
{
    Request             *req = NULL;
    Response            *res = NULL;
    Config              *cfg = NULL;
    ApplicationResult   *app = NULL;
};

class WebservEvent
{
    public:
        //struct Entity;
        //WebservEvent();
        virtual ~WebservEvent(){};
        virtual EWebservEvent which() = 0;
        //static WebservEvent *from_epoll_event(t_epoll_event const &event);

        virtual FileDiscriptor &fd() = 0;
        virtual WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory) = 0;
        virtual E_EpollEvent get_next_epoll_event() = 0;

        virtual Request *req() = 0;
        virtual Response *res() = 0;
        virtual File *src() = 0;
        virtual File *dst() = 0;
        //virtual void set_io(uint32_t epoll_event) = 0;
        virtual void set_src(File *file) = 0;
        virtual void set_dst(File *file) = 0;

        virtual bool is_completed() = 0;
        virtual void set_completed(bool flag) = 0;

        virtual void increase_timeout_count(int count) = 0;
        virtual int  timeout_count() = 0;

        virtual Entity *entity() = 0;
        //virtual void set_cgi_event(WebservCgiEvent *cgi_event) = 0;
        //virtual WebservCgiEvent *cgi_event() = 0;

    private:
        Entity *entity_;
        Request *req_;
        Response *res_;
        int timeout_count_;

};

#endif
