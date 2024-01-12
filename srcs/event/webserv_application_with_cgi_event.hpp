#ifndef WEBSERV_APPLICATION_WITH_CGI_EVENT_HPP
#define WEBSERV_APPLICATION_WITH_CGI_EVENT_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"
#include "webserv_cgi_event.hpp"
#include "application.hpp"
#include "application_result.hpp"
#include "webserv_application_event.hpp"

class WebservApplicationWithCgiEvent : public WebservEvent, public WebservApplicationEvent
{
    public:
        WebservApplicationWithCgiEvent(FileDiscriptor fd, Request *req);
        ~WebservApplicationWithCgiEvent();
        static WebservApplicationWithCgiEvent *from_event(WebservEvent *event);
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

        void set_result(ApplicationResult *result);
        ApplicationResult *result();

    private:
        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        File            *file_;
        File            *source_file;
        File            *destination_file;

        int             timeout_count_;
        bool            is_completed_;

        WebservCgiEvent *cgi_event_;
        ApplicationResult *result_;
};

#endif
