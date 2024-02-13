
#ifndef WEBSERV_EVENT_NOTHING_HPP
#define WEBSERV_EVENT_NOTHING_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_cgi_event.hpp"
#include "webserv_entity.hpp"

class WebservNothingEvent
{
    public:
        WebservNothingEvent();
        //WebservNothingEvent(FileDiscriptor &fd);
        ~WebservNothingEvent();
        EWebservEvent which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);

        static WebservEvent *make_nothing_event();
        //FileDiscriptor  &fd();
        //Request         *req();
        //Response        *res();
        //File            *src();
        //File            *dst();
        //void            switching_io(uint32_t epoll_event);
        //void            set_src(File *file);
        //void            set_dst(File *file);
        //bool is_completed();
        //void set_completed(bool flag);
        //void increase_timeout_count(int count);
        //int  timeout_count();

        //void set_cgi_event(WebservCgiEvent *cgi_event);
        //WebservCgiEvent *cgi_event();
        //WebservEntity *entity();
        

    private:
        //int             timeout_count_;
        //FileDiscriptor fd_;
        //bool is_completed_;
        //WebservCgiEvent *cgi_event_;
        //WebservEntity         *entity_;
};
#endif
