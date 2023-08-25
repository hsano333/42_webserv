#include "epoll.hpp"
#include "global.hpp"
#include "utility.hpp"
#include <sys/epoll.h>

Epoll::Epoll()
{
    ;
}

Epoll::Epoll(Epoll const &epoll)
{
    std::cout << "copy constructo" << std::endl;
    *this = epoll;
    std::cout << this->allocated_event_size() << std::endl;
    std::cout << epoll.allocated_event_size() << std::endl;
}

Epoll::~Epoll()
{
    ;
}

Epoll& Epoll::operator=(Epoll const & epoll)
{
    std::cout << "operator=" << std::endl;
    this->epfd = epoll.epfd;
    this->allocated_events_space.resize(epoll.allocated_event_size());
    for(size_t i=0;i<this->allocated_events_space.size();i++){
        this->allocated_events_space[i] = epoll.allocated_events_space[i];
    }
    return *this;
}


/*
Epoll Epoll::from_sockets(SocketRepository const &socket_repository)
{
    //(void)sockets;

    Epoll object;
    object.init();
    //object.resize_allocated_event_size(size);
    return (object);
}
*/

void Epoll::init()
{
    //10 is ignored; reference man;
    int tmp_fd = epoll_create(10);
    this->epfd = FileDiscriptor::from_int(tmp_fd);


    if(this->epfd < 0){
        ERROR("Epoll Create Error");
        std::runtime_error("Epoll Create Error:");
    }
}

FileDiscriptor Epoll::fd()
{
    return (this->epfd);
}

t_epoll_event *Epoll::allocated_event_pointer()
{
    return &(this->allocated_events_space[0]);
}

t_epoll_event const* Epoll::event_related_with_fd(FileDiscriptor fd)
{
    std::vector<t_epoll_event>::const_iterator ite = this->allocated_events_space.begin();
    std::vector<t_epoll_event>::const_iterator end = this->allocated_events_space.end();
    while(ite != end){
        const t_epoll_event &ev = *ite;
        if(fd == ev.data.fd){
            return &(*ite);
        }
        ite++;
    }
    WARNING("There is no event related with fd:" + Utility::to_string(fd));
    throw std::invalid_argument("There is no event related with fd");
    //return (NULL);
    //return (*ite);
}


size_t Epoll::allocated_event_size() const
{
    return (this->allocated_events_space.size());
}

std::vector<t_epoll_event> &Epoll::get_events()
{
    return (this->allocated_events_space);

}

/*
void Epoll::resize_allocated_event_size(size_t size)
{
    this->allocated_events_space.resize(size);
}
*/

void Epoll::save_executable_events_number(int size)
{
    if(size > 0){
        this->executable_event_number = size;
    }else if(size == 0){
        //timeout 

    }else if(size < 0){
        ERROR("Epoll Wait Error nfds < 0");
        throw std::runtime_error("Epoll Wait Error nfds < 0");
    }
}

void Epoll::expand_allocated_space()
{
    size_t size = this->allocated_events_space.size()+1;
    this->allocated_events_space.resize(size);
}


void Epoll::contract_allocated_space()
{
    size_t size = this->allocated_events_space.size()-1;
    if(size > 0){
        this->allocated_events_space.resize(size);
    }else{
        WARNING("Epoll::contract_allocated_space Error. can't resize 0");
        throw std::runtime_error("Epoll::contract_allocated_space Error. can't resize 0");
    }
}

#ifdef UNIT_TEST
#include "doctest.h"
TEST_CASE("Epoll Class")
{
    Epoll epoll = Epoll::from_socket_size(5);
    CHECK(epoll.allocated_event_size() == 0);
    epoll.expand_allocated_space();
    CHECK(epoll.allocated_event_size() == 1);
    epoll.expand_allocated_space();
    CHECK(epoll.allocated_event_size() == 2);
    epoll.contract_allocated_space();
    CHECK(epoll.allocated_event_size() == 1);
    ;
    CHECK_THROWS_AS(epoll.contract_allocated_space(), std::runtime_error);


    close(epoll.fd());
}

#endif
