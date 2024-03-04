#ifndef WEBSERV_MAKE_RESPONSE_EVENT_HPP
#define WEBSERV_MAKE_RESPONSE_EVENT_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"
#include "config.hpp"
#include "webserv_entity.hpp"
#include "webserv_file_factory.hpp"

class WebservMakeResponseEvent
{
    public:
        ~WebservMakeResponseEvent();
        static WebservEvent *from_event(WebservEvent *event, WebservFile *src, WebservFile *dst);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);
        Response  *make(WebservEntity *entity);

    private:
        WebservMakeResponseEvent();
        static WebservMakeResponseEvent *singleton;
        static WebservMakeResponseEvent *get_instance();
        Response *make_response(ApplicationResult *result);
        Response *make_response_for_cgi(ApplicationResult *result, WebservEntity *entity);
        bool check_body_size(Request *req, const ConfigServer *server);
        void parse_req(WebservEvent *event);
};

#endif
