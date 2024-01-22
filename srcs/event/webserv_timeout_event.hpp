#ifndef WEBSERV_TIMEOUT_EVENT_HPP
#define WEBSERV_TIMEOUT_EVENT_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "webserv_cgi_event.hpp"

class WebservTimeoutEvent : public WebservEvent
{
    public:
        WebservTimeoutEvent();
        WebservTimeoutEvent(FileDiscriptor fd);
        ~WebservTimeoutEvent();
        EWebservEvent which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();

        FileDiscriptor  &fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        void            switching_io(uint32_t epoll_event);
        void            set_src(File *file);
        void            set_dst(File *file);
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();

    private:
        FileDiscriptor fd_;
        EWebservEvent event_type;
        int timeout_count_;
        bool is_completed_;
        WebservCgiEvent *cgi_event_;
};
#endif
