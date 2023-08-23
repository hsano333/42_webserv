#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <vector>
#include <unistd.h>
#include "socket.hpp"
typedef struct epoll_event t_epoll_event;
//DTO? entity?
class Epoll
{
    public:
        Epoll();
        Epoll(Epoll const &epoll);
        ~Epoll();
        //static Epoll from_sockets(SocketRepository const &socket_repository);
        Epoll& operator=(Epoll const & epoll);
        void init();
        int fd();
        t_epoll_event *returned_events_pointer();
        t_epoll_event const *event_related_with_fd(int fd);
        size_t allocated_event_size() const;
        //void resize_allocated_event_size(size_t size);
        void save_executable_events_number(int number);
        void expand_allocated_space();
        void contract_allocated_space();
    private:
        int epfd;
        int executable_event_number;
        std::vector<t_epoll_event> allocated_events_space;
};

#endif
