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
    EPOLL_FOR_CGI,
    EPOLL_CONTINUE, //5
    EPOLL_CLOSE,
    EPOLL_CGI_IN,
    EPOLL_CGI_OUT,
    EPOLL_CGI_STOP,
    EPOLL_CGI, //10
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
        //t_epoll_event const *event_related_with_fd(FileDiscriptor fd);
        //size_t allocated_event_size() const;
        void save_executable_events_number(int number);
        //void expand_allocated_space();
        //void contract_allocated_space();
        //std::vector<t_epoll_event> &get_events();
        int executable_event_number();
        //t_epoll_event *event_from_fd(int fd);
    private:
        FileDiscriptor epfd;
        int executable_event_number_;
        t_epoll_event event_return_[EVENT_MAX_NUMBER];
};

#endif
