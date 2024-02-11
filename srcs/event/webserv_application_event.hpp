#ifndef WEBSERV_APPLICATION_EVENT_HPP
#define WEBSERV_APPLICATION_EVENT_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"
#include "webserv_cgi_event.hpp"
#include "application.hpp"
#include "webserv_entity.hpp"

class WebservApplicationEvent
{
    public:
        //WebservApplicationEvent(FileDiscriptor fd, Request *req);
        ~WebservApplicationEvent(){};
        virtual WebservEntity *entity() = 0;
        //virtual Request         *req() = 0;
        /*
        static WebservApplicationEvent *from_event(WebservEvent *event);
        EWebservEvent   which();
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        E_EpollEvent get_next_epoll_event();
        FileDiscriptor  fd();
        Request         *req();
        Response        *res();
        File            *src();
        File            *dst();
        void            set_src(File *file);
        void            set_dst(File *file);

        bool is_completed();
        void set_completed(bool flag);

        void increase_timeout_count(int count);
        int  timeout_count();
        void            set_response(Response *res);
        File *file();
        void set_file(File *file);

        void set_cgi_event(WebservCgiEvent *cgi_event);
        WebservCgiEvent *cgi_event();
        */

    private:
        /*
        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        File            *file_;
        File            *source_file;
        File            *destination_file;

        int             timeout_count_;
        bool            is_completed_;

        WebservCgiEvent *cgi_event_;
        Application app;
        */
};

#endif
