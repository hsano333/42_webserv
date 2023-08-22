#ifndef EPOLL_HPP
#define EPOLL_HPP

#include <vector>
#include <unistd.h>
typedef struct epoll_event t_epoll_event;
//DTO? entity?
class Epoll
{
    public:
        ~Epoll();
        static Epoll from_socket_size(size_t size);
        Epoll& operator=(Epoll const & epoll);
        int fd();
        t_epoll_event *returned_events_pointer();
        t_epoll_event const *event_related_with_fd(int fd);
        size_t event_size();
        void change_event_size(size_t size);
        void set_event_number(int number);
        void expand_events();
        void contract_events();
    private:
        Epoll();
        void init();
        int epfd;
        int event_number;
        std::vector<t_epoll_event> executable_events;
};

#endif
