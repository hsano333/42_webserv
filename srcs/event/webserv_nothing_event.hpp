
#ifndef WEBSERV_EVENT_NOTHING_HPP
#define WEBSERV_EVENT_NOTHING_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_entity.hpp"

class WebservNothingEvent
{
    public:
        ~WebservNothingEvent();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);

        static WebservEvent *make_nothing_event();
        static void delete_myself();
    private:
        WebservNothingEvent();
        static WebservNothingEvent *singleton;
        static WebservNothingEvent *get_instance();
};
#endif
