#ifndef WEBSERV_EVENT_KEEP_ALIVE_HPP
#define WEBSERV_EVENT_KEEP_ALIVE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_cgi_event.hpp"
#include "webserv_io_event.hpp"
#include "webserv_entity.hpp"

class WebservKeepAliveEvent
{
    public:
        ~WebservKeepAliveEvent();
        EWebservEvent which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);

        static WebservEvent *from_event(WebservEvent *event);

    private:
        WebservKeepAliveEvent();
        static WebservKeepAliveEvent *singleton;
        static WebservKeepAliveEvent *get_instance();
};
#endif
