
#ifndef WEBSERV_EVENT_NOTHING_HPP
#define WEBSERV_EVENT_NOTHING_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"

class WebservNothingEvent : public WebservEvent
{
    public:
        WebservNothingEvent();
        ~WebservNothingEvent();
        EWebservEvent which();

        static WebservNothingEvent *from_fd(FileDiscriptor fd);
        FileDiscriptor fd();
        Request *req();
        Response *res();
        void increase_timeout_count(int count);
        int  timeout_count();
    private:
        int             timeout_count_;
};
#endif
