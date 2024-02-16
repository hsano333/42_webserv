
#ifndef WEBSERV_EVENT_NOTHING_HPP
#define WEBSERV_EVENT_NOTHING_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_entity.hpp"

class WebservNothingEvent
{
    public:
        WebservNothingEvent();
        ~WebservNothingEvent();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);

        static WebservEvent *make_nothing_event();
    private:
};
#endif
