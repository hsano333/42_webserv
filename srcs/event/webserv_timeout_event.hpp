#ifndef WEBSERV_TIMEOUT_EVENT_HPP
#define WEBSERV_TIMEOUT_EVENT_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "webserv_entity.hpp"
#include "webserv_cleaner.hpp"

class WebservTimeoutEvent
{
    public:
        ~WebservTimeoutEvent();
        static WebservEvent *make(WebservEvent *event);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        FDManager *fd_manager;
        EventManager *event_manager;
        void        check_completed(WebservEntity * entity);
        WebservEvent *from_event(WebservEvent *event);
        

    private:
        WebservTimeoutEvent();
        static WebservTimeoutEvent *singleton;
        static WebservTimeoutEvent *get_instance();
};
#endif
