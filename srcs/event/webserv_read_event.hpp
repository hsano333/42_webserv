#ifndef WEBSERV_EVENT_READ_HPP
#define WEBSERV_EVENT_READ_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"

class WebservReadEvent : public WebservEvent
{
    public:
        WebservReadEvent();
        //WebservReadEvent(FileDiscriptor fd, IReader* ireader);
        ~WebservReadEvent();
        EWebservEvent which();

        //static WebservReadEvent *from_fd(FileDiscriptor fd, IReader *reader);
        static WebservReadEvent *from_fd(FileDiscriptor fd);
        static WebservReadEvent *from_event(WebservEvent *event);
        FileDiscriptor fd();


        Request *req();
        Response *res();
        bool is_end();
        void set_end(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        //int read(char *buf, size_t size);


    private:
        WebservReadEvent(FileDiscriptor fd);
        Request *req_;
        Response *res_;
        FileDiscriptor fd_;
        //EWebservEvent event_type;
        int timeout_count_;
        bool is_end_;
        //IReader *ireader;
};
#endif
