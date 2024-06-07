#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <vector>
#include <unistd.h>
#include "socket.hpp"
#define EVENT_MAX_NUMBER 250

typedef enum E_EpollEvent
{
    EPOLL_READ,
    EPOLL_WRITE,
    EPOLL_ADD_READ,
    EPOLL_ADD_WRITE,
    EPOLL_FOR_CGI_POST,
    EPOLL_READ_FOR_POST_CGI,
    EPOLL_WRITE_FOR_POST_CGI,
    EPOLL_FOR_CGI_WAIT_CGI,
    EPOLL_FOR_CGI_GET_OUT,
    EPOLL_CONTINUE,
    EPOLL_CLOSE,
    EPOLL_CGI_IN,
    EPOLL_CGI_OUT,
    EPOLL_CGI_STOP,
    EPOLL_CGI,
    EPOLL_NONE,
} E_EpollEvent;


typedef struct epoll_event t_epoll_event;
class Epoll
{
    public:
        Epoll();
        Epoll(Epoll const &epoll);
        ~Epoll();
        Epoll& operator=(Epoll const & epoll);
        void init();
        FileDiscriptor const &fd();
        t_epoll_event *event_return();
        void save_executable_events_number(int number);
        int executable_event_number();
    private:
        FileDiscriptor epfd;
        int executable_event_number_;
        t_epoll_event event_return_[EVENT_MAX_NUMBER];
};

#endif
