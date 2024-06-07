#include "epoll.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <sys/epoll.h>

Epoll::Epoll()
{
}

Epoll::Epoll(Epoll const &epoll)
{
    *this = epoll;
}

Epoll::~Epoll()
{
    ;
}

Epoll& Epoll::operator=(Epoll const & epoll)
{
    DEBUG("Epoll::operator=");
    if (this == &epoll){
        return (*this);
    }
    this->epfd = epoll.epfd;
    this->executable_event_number_ = epoll.executable_event_number_;
    for(int i=0;i<EVENT_MAX_NUMBER;i++){
        this->event_return_[i] = epoll.event_return_[i];
    }
    return *this;
}

void Epoll::init()
{
    //10 is ignored; reference to man;
    int tmp_fd = epoll_create(10);
    this->epfd = FileDiscriptor::from_int(tmp_fd);
}

FileDiscriptor const &Epoll::fd()
{
    return (this->epfd);
}

t_epoll_event *Epoll::event_return()
{
    return &(this->event_return_[0]);
}


int Epoll::executable_event_number()
{
    DEBUG("executable_event_number: " + Utility::to_string(this->executable_event_number_));
    return (this->executable_event_number_);
}

void Epoll::save_executable_events_number(int size)
{
    DEBUG("Epoll::save_executable_events_number():" + Utility::to_string(size));
    if(size >= 0){
        this->executable_event_number_ = size;
    }else if(size < 0){
        ERROR("Epoll Wait Error nfds < 0");
        throw std::runtime_error("Epoll Wait Error nfds < 0");
    }
}

