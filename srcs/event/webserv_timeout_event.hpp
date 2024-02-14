#ifndef WEBSERV_TIMEOUT_EVENT_HPP
#define WEBSERV_TIMEOUT_EVENT_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "webserv_cgi_event.hpp"
#include "webserv_entity.hpp"

class WebservTimeoutEvent
{
    public:
        WebservTimeoutEvent(FileDiscriptor fd);
        ~WebservTimeoutEvent();
        static WebservEvent *make();
        EWebservEvent which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);

    private:
        WebservTimeoutEvent();
        static WebservTimeoutEvent *singleton;
        static WebservTimeoutEvent *get_instance();
};
#endif
