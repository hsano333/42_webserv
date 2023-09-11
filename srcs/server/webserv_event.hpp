#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP
#include "epoll.hpp"
#include "file_discriptor.hpp"

typedef enum E_WebservEvent
{
    READ_EVENT,
    APPLICATION_EVENT,
    WRITE_EVENT,
    TIMEOUT_EVENT,
    NOTHING_EVENT,
} EWebservEvent;

class WebservEvent
{
    public:
        //WebservEvent();
        virtual ~WebservEvent(){};
        virtual EWebservEvent which() = 0;
        //static WebservEvent *from_epoll_event(t_epoll_event const &event);

        virtual FileDiscriptor get_fd() = 0;
        virtual void increase_and_check_timeout_count(int count) = 0;
        Request *req;
        Response *res;

    private:
        FileDiscriptor fd;
        //EWebservEvent event_type;
        std::string filepath;
        int timeout_count;
};

#endif
