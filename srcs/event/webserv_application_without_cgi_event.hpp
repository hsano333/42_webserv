#ifndef WEBSERV_APPLICATION_WITHOUT_CGI_EVENT_HPP
#define WEBSERV_APPLICATION_WITHOUT_CGI_EVENT_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"
//#include "application.hpp"
#include "application_result.hpp"
#include "webserv_application_event.hpp"
#include "webserv_entity.hpp"
#include "webserv_cleaner.hpp"

class WebservApplicationWithoutCgiEvent
{
    public:
        ~WebservApplicationWithoutCgiEvent();
        static WebservEvent *from_event(WebservEvent *event);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        void         set_next_epoll_event(E_EpollEvent epoll_event);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);

    private:
        static WebservApplicationWithoutCgiEvent *singleton;
        static WebservApplicationWithoutCgiEvent *get_instance();
        WebservApplicationWithoutCgiEvent();
};

#endif
