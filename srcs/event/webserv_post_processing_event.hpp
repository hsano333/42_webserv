#ifndef WEBSERV_POST_PROCESSING_EVENT
#define WEBSERV_POST_PROCESSING_EVENT
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"

class WebservPostProcessingEvent : public WebservEvent
{
    public:
        WebservPostProcessingEvent(
                            FileDiscriptor fd,
                            Request *req,
                            Response *res
                );
        ~WebservPostProcessingEvent();
        EWebservEvent which();

        static WebservPostProcessingEvent *from_fd(FileDiscriptor fd);
        FileDiscriptor get_fd();
        void increase_and_check_timeout_count(int count);
        static WebservPostProcessingEvent *from_webserv_event(WebservEvent *event);
        Request *req();
        Response *res();
    private:
        FileDiscriptor fd;
        Request *req_;
        Response *res_;


};

#endif
