#ifndef WEBSERV_CLEAN_EVENT
#define WEBSERV_CLEAN_EVENT
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"

class WebservCleanEvent : public WebservEvent
{
    public:
        WebservCleanEvent(
                            FileDiscriptor fd,
                            Request *req,
                            Response *res
                );
        ~WebservCleanEvent();
        EWebservEvent which();

        static WebservCleanEvent *from_fd(FileDiscriptor fd);
        FileDiscriptor fd();
        void increase_timeout_count(int count);
        int  timeout_count();
        static WebservCleanEvent *from_webserv_event(WebservEvent *event, bool force_close);
        Request *req();
        Response *res();
        bool    is_force_close();
    private:
        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        int             timeout_count_;
        bool            force_close;
};

#endif
