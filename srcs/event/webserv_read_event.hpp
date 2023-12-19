#ifndef WEBSERV_EVENT_READ_HPP
#define WEBSERV_EVENT_READ_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_cgi_event.hpp"

class WebservReadEvent : public WebservEvent
{
    public:
        WebservReadEvent();
        //WebservReadEvent(FileDiscriptor fd, IReader* ireader);
        ~WebservReadEvent();
        EWebservEvent which();

        //static WebservReadEvent *from_fd(FileDiscriptor fd, IReader *reader);
        static WebservReadEvent *from_fd(FileDiscriptor fd, FileDiscriptor sockfd, IReader *reader);
        static WebservReadEvent *from_cgi_fd(FileDiscriptor sockfd, IReader *reader);
        static WebservReadEvent *from_event(WebservEvent *event, FileDiscriptor sockfd, IReader *reader);
        FileDiscriptor fd();


        Request *req();
        Response *res();
        bool is_completed();
        void set_completed(bool flag);
        void increase_timeout_count(int count);
        int  timeout_count();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        int read(char *buf, size_t size);

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();

    private:
        WebservReadEvent(FileDiscriptor fd, IReader *reader);
        Request *req_;
        Response *res_;
        FileDiscriptor fd_;
        //EWebservEvent event_type;
        int timeout_count_;
        bool is_completed_;
        IReader *reader;
        WebservCgiEvent *cgi_event_;
};
#endif
