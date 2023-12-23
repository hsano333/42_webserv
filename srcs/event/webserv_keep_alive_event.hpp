
#ifndef WEBSERV_EVENT_KEEP_ALIVE_HPP
#define WEBSERV_EVENT_KEEP_ALIVE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_cgi_event.hpp"

class WebservKeepAliveEvent : public WebservEvent
{
    public:
        WebservKeepAliveEvent();
        ~WebservKeepAliveEvent();
        EWebservEvent which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();

        static WebservKeepAliveEvent *from_fd(FileDiscriptor fd);
        FileDiscriptor  fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();
    private:
        int             timeout_count_;
        FileDiscriptor fd_;
        WebservKeepAliveEvent(FileDiscriptor fd);
        //bool is_end_;
        WebservCgiEvent *cgi_event_;
};
#endif
