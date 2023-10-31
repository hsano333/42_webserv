
#ifndef WEBSERV_EVENT_NOTHING_HPP
#define WEBSERV_EVENT_NOTHING_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"

class WebservNothingEvent : public WebservEvent
{
    public:
        WebservNothingEvent();
        ~WebservNothingEvent();
        EWebservEvent which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();

        static WebservNothingEvent *from_fd(FileDiscriptor fd);
        FileDiscriptor fd();
        Request *req();
        Response *res();
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();
    private:
        int             timeout_count_;
        FileDiscriptor fd_;
        WebservNothingEvent(FileDiscriptor fd);
        //bool is_completed_;
};
#endif
