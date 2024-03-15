#ifndef WEBSERV_EVENT_KEEP_ALIVE_HPP
#define WEBSERV_EVENT_KEEP_ALIVE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_entity.hpp"

//class WebservEvent;
class WebservKeepAliveEvent
{
    public:
        ~WebservKeepAliveEvent();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        static WebservEvent *from_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);

    private:
        WebservKeepAliveEvent();
        static WebservKeepAliveEvent *singleton;
        static WebservKeepAliveEvent *get_instance();
};
#endif
