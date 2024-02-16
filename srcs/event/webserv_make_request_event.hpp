#ifndef WEBSERV_MAKE_REQUEST_EVENT_HPP
#define WEBSERV_MAKE_REQUEST_EVENT_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"
#include "config.hpp"
#include "webserv_entity.hpp"

class WebservMakeRequestEvent
{
    public:
        ~WebservMakeRequestEvent();
        static WebservEvent *from_event(WebservEvent *event, File *src, File *dst);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event(WebservEvent *event);


        Request *make_request(WebservEntity *entity);
        void    set_file(File *file);

    private:
        WebservMakeRequestEvent();
        static WebservMakeRequestEvent *singleton;
        static WebservMakeRequestEvent *get_instance();

        void parse_request(Request *req, File *src);
        bool check_cgi(const Request *req, const ConfigLocation *location) const;
        bool check_body_size(Request *req, const ConfigServer *server);
        void parse_req(WebservEvent *event);

};

#endif
