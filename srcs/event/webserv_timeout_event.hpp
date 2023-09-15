#ifndef WEBSERV_TIMEOUT_EVENT_HPP
#define WEBSERV_TIMEOUT_EVENT_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"

class WebservTimeoutEvent : public WebservEvent
{
    public:
        WebservTimeoutEvent();
        WebservTimeoutEvent(FileDiscriptor fd);
        ~WebservTimeoutEvent();
        EWebservEvent which();

        FileDiscriptor get_fd();
        Request *req();
        Response *res();
        void increase_timeout_count(int count);
        int  timeout_count();

    private:
        FileDiscriptor fd;
        EWebservEvent event_type;
        int timeout_count_;
};
#endif
