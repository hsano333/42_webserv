#ifndef WEBSERV_APPLICATION_EVENT_HPP
#define WEBSERV_APPLICATION_EVENT_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"

class WebservApplicationEvent : public WebservEvent
{
    public:
        WebservApplicationEvent(FileDiscriptor fd, Request *req);
        ~WebservApplicationEvent();
        static WebservApplicationEvent *from_event(WebservEvent *event);
        EWebservEvent   which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        FileDiscriptor  fd();
        Request         *req();
        Response        *res();

        bool is_completed();
        void set_completed(bool flag);

        void increase_timeout_count(int count);
        int  timeout_count();
        void            set_response(Response *res);
        File *file();
        void set_file(File *file);
    private:
        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        File            *file_;
        int             timeout_count_;
        bool            is_completed_;



};

#endif
