#ifndef WEBSERV_EVENT_READ_HPP
#define WEBSERV_EVENT_READ_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"

class WebservReadEvent : public WebservEvent
{
    public:
        WebservReadEvent();
        WebservReadEvent(FileDiscriptor fd);
        WebservReadEvent(FileDiscriptor fd, IReader* ireader);
        ~WebservReadEvent();
        EWebservEvent which();

        static WebservReadEvent *from_fd(FileDiscriptor fd, IReader *reader);
        FileDiscriptor get_fd();
        Request *req();
        Response *res();
        void increase_timeout_count(int count);
        int  timeout_count();
        int read(char *buf, size_t size);


    private:
        Request *req_;
        Response *res_;
        FileDiscriptor fd;
        //EWebservEvent event_type;
        int timeout_count_;
        IReader *ireader;
};
#endif
