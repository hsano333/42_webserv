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
        static WebservEvent *make(WebservCleaner *cleaner_);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);
        FDManager *fd_manager;
        EventManager *event_manager;
        WebservCleaner *cleaner() const;
        void        check_completed(WebservEntity * entity);

    private:
        WebservTimeoutEvent();
        static WebservTimeoutEvent *singleton;
        static WebservTimeoutEvent *get_instance(WebservCleaner *cleaner);
        WebservCleaner *cleaner_;
};
#endif
