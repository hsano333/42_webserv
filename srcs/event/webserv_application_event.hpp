#ifndef WEBSERV_APPLICATION_EVENT_HPP
#define WEBSERV_APPLICATION_EVENT_HPP
#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"

class WebservApplicationEvent : public WebservEvent
{
    public:
        WebservApplicationEvent(FileDiscriptor fd, Request *req);
        ~WebservApplicationEvent();
        static WebservApplicationEvent *from_event(WebservEvent *event);
        EWebservEvent   which();
        FileDiscriptor  get_fd();
        Request         *req();
        Response        *res();
        void increase_timeout_count(int count);
        int  timeout_count();
    private:
        FileDiscriptor  fd;
        Request         *req_;
        //Response        *res_;
        int             timeout_count_;



};

#endif
