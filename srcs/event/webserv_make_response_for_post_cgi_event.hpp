#ifndef WEBSERV_MAKE_RESPONSE_FOR_POST_CGI_EVENT_HPP
#define WEBSERV_MAKE_RESPONSE_FOR_POST_CGI_EVENT_HPP
#include "webserv_event.hpp"
#include "webserv_io_event.hpp"
#include "file_discriptor.hpp"
#include "iwriter.hpp"
#include "ireader.hpp"
#include "webserv_file.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_entity.hpp"

class WebservMakeResponseForPostCGIEvent
{
    public:
        ~WebservMakeResponseForPostCGIEvent();
        static WebservEvent *from_event(WebservEvent *event);
        //static WebservEvent *from_fd(FileDiscriptor &write_fd, FileDiscriptor &read_fd, WebservFile *read_src, WebservFile *read_dst, WebservFile *write_src, WebservFile *write_dst, WebservEvent *event);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);
        Response* make_response_for_cgi(ApplicationResult *result, WebservEntity *entity);

        //static WebservMakeResponseForPostCGIEvent *from_event(WebservEvent *event, WebservFile *io, WebservFile *write_src, WebservFile *read_dst);
    private:
        WebservMakeResponseForPostCGIEvent();
        static WebservMakeResponseForPostCGIEvent *singleton;
        static WebservMakeResponseForPostCGIEvent *get_instance();
        WebservMakeResponseForPostCGIEvent(FileDiscriptor  &fd, FileDiscriptor  &sockfd);
};
#endif
