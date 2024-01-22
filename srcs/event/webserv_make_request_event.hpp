#ifndef WEBSERV_MAKE_REQUEST_EVENT_HPP
#define WEBSERV_MAKE_REQUEST_EVENT_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"
#include "webserv_cgi_event.hpp"
#include "webserv_make_event.hpp"
#include "config.hpp"

class WebservMakeRequestEvent : public  IWebservMakeEvent, public WebservEvent
{
    public:
        //WebservMakeRequestEvent(FileDiscriptor fd, Request *req);
        ~WebservMakeRequestEvent();
        static WebservMakeRequestEvent *from_event(WebservEvent *event, IReader *reader, Config *cfg, File *src, File *dst);
        EWebservEvent   which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        FileDiscriptor  &fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        void            switching_io(uint32_t epoll_event);
        void            set_src(File *file);
        void            set_dst(File *file);

        bool is_completed();
        void set_completed(bool flag);

        void increase_timeout_count(int count);
        int  timeout_count();
        //void            set_response(Response *res);
        //File *file();

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();

        File    *make_request();
        File    *make();
        void    set_file(File *file);

    private:
        WebservMakeRequestEvent(FileDiscriptor fd, Request *req, IReader *reader, Config *cfg);

        void parse_request(Request *req);
        bool check_cgi(const Request *req, const ConfigLocation *location) const;



        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        File            *file_;
        File            *source_file;
        File            *destination_file;

        int             timeout_count_;
        bool            is_completed_;

        WebservCgiEvent *cgi_event_;

        bool check_body_size(Request *req, const ConfigServer *server);
        void parse_req(WebservEvent *event);
        IReader *reader;
        Config *cfg;
        //bool is_cgi;

};

#endif
