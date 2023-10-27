#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP
#include "epoll.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_event_factory.hpp"

typedef enum E_WebservEvent
{
    READ_EVENT,
    APPLICATION_EVENT,
    WRITE_EVENT,
    CLEAN_EVENT,
    TIMEOUT_EVENT,
    KEEPA_ALIVE_EVENT,
    NOTHING_EVENT,
} EWebservEvent;

typedef enum E_EpollEvent
{
    EPOLL_READ,
    EPOLL_WRITE,
    EPOLL_CONTINUE,
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

        virtual bool is_end() = 0;
        virtual void set_end(bool flag) = 0;

        virtual void increase_timeout_count(int count) = 0;
        virtual int  timeout_count() = 0;

    private:
        Request *req_;
        Response *res_;
        FileDiscriptor fd_;
        //EWebservEvent event_type;
        std::string filepath;
        int timeout_count_;
};

#endif
