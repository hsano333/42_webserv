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
        FileDiscriptor fd();
        t_epoll_event *allocated_event_pointer();
        t_epoll_event const *event_related_with_fd(FileDiscriptor fd);
        size_t allocated_event_size() const;
        //void resize_allocated_event_size(size_t size);
        void save_executable_events_number(int number);
        void expand_allocated_space();
        //void expand_allocated_space(t_epoll_event tmp);
        void contract_allocated_space();
        std::vector<t_epoll_event> &get_events();
        int executable_event_number();
        t_epoll_event *event_from_fd(int fd);
    private:
        FileDiscriptor epfd;
        int executable_event_number_;
        std::vector<t_epoll_event> allocated_events_space;
};

#endif
