#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP
#include "epoll.hpp"
#include "file_discriptor.hpp"

typedef enum E_WebservEvent
{
    READ_EVENT,
    APPLICATION_EVENT,
    WRITE_EVENT,
} EWebservEvent;

class WebservEvent
{
    public:
        WebservEvent();
        ~WebservEvent();
        EWebservEvent which();
        static WebservEvent *from_epoll_event(t_epoll_event const &event);

        FileDiscriptor get_fd();

    private:
        FileDiscriptor fd;
        EWebservEvent event_type;


};

#endif
