#ifndef WEBSERV_ERROR_EVENT_HPP
#define WEBSERV_ERROR_EVENT_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"
#include "webserv_entity.hpp"

//class WebservEvent;
class WebservErrorEvent
{
    public:
        ~WebservErrorEvent();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        static WebservEvent *from_event(WebservEvent *event, StatusCode &code);
        void        check_completed(WebservEntity * entity);
        static void delete_myself();

    private:
        WebservErrorEvent();
        static WebservErrorEvent *singleton;
        static WebservErrorEvent *get_instance();
};
#endif
