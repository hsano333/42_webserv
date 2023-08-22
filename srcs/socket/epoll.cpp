#include "epoll.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <sys/epoll.h>

Epoll::Epoll()
{
    ;
}

Epoll::~Epoll()
{
    ;
}

Epoll& Epoll::operator=(Epoll const & epoll)
{
    this->epfd = epoll.epfd;
    this->executable_events.resize(this->executable_events.size());
    for(size_t i=0;i<this->executable_events.size();i++){
        this->executable_events[i] = this->executable_events[i];
    }
    return *this;
}

Epoll Epoll::from_socket_size(size_t size)
{
    Epoll epoll;
    epoll.init();
    epoll.executable_events.resize(size);
    return (epoll);
}

void Epoll::init()
{
    //10 is ignored; reference man;
    this->epfd = epoll_create(10);
    if(this->epfd < 0){
        ERROR("Epoll Create Error");
        std::runtime_error("Epoll Create Error:");
    }
}

int Epoll::fd()
{
    return (this->epfd);
}

t_epoll_event *Epoll::returned_events_pointer()
{
    return &(this->executable_events[0]);
}

t_epoll_event const* Epoll::event_related_with_fd(int fd)
{
    std::vector<t_epoll_event>::const_iterator ite = this->executable_events.begin();
    std::vector<t_epoll_event>::const_iterator end = this->executable_events.end();
    while(ite != end){
        const t_epoll_event &ev = *ite;
        if(ev.data.fd == fd){
            return &(*ite);
        }
        ite++;
    }
    WARNING("There is no event related with fd:" + Utility::to_string(fd));
    throw std::invalid_argument("There is no event related with fd");
    //return (NULL);
    //return (*ite);
}


size_t Epoll::event_size()
{
    return (this->executable_events.size());
}

void Epoll::change_event_size(size_t size)
{
    this->executable_events.resize(size);
}

void Epoll::set_event_number(int size)
{
    if(size > 0){
        this->event_number = size;
    }else if(size == 0){
        //timeout 

    }else if(size < 0){
        ERROR("Epoll Wait Error nfds < 0");
        throw std::runtime_error("Epoll Wait Error nfds < 0");
    }
}

void Epoll::expand_events()
{
    size_t size = this->executable_events.size()+1;
    this->executable_events.resize(size);
}


void Epoll::contract_events()
{
    size_t size = this->executable_events.size()-1;
    if(size > 0){
        this->executable_events.resize(size);
    }else{
        WARNING("Epoll::contract_events Error. can't resize 0");
        throw std::runtime_error("Epoll::contract_events Error. can't resize 0");
    }
}


