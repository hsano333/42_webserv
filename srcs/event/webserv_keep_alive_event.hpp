
#ifndef WEBSERV_EVENT_KEEP_ALIVE_HPP
#define WEBSERV_EVENT_KEEP_ALIVE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"

class WebservKeepAliveEvent : public WebservEvent
{
    public:
        WebservKeepAliveEvent();
        ~WebservKeepAliveEvent();
        EWebservEvent which();

        static WebservKeepAliveEvent *from_fd(FileDiscriptor fd);
        FileDiscriptor fd();
        Request *req();
        Response *res();
        void increase_timeout_count(int count);
        int  timeout_count();
    private:
        int             timeout_count_;
        FileDiscriptor fd_;
        WebservKeepAliveEvent(FileDiscriptor fd);
};
#endif
