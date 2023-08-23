#ifndef EPOLL_CONTROLLER_HPP
#define EPOLL_CONTROLLER_HPP

#include <vector>
#include <map>
#include <sys/epoll.h>
#include "epoll.hpp"
#include "event_manager.hpp"
#include "io_multiplexing.hpp"
#include "socket_manager.hpp"

class EpollController : public IOMultiplexing
{
    public:
        EpollController(Epoll epoll,SocketRepository *socket_repository);
        ~EpollController();
        void wait();
        void add(int fd, uint32_t event);
        void erase(int fd);
        void modify(int fd, uint32_t event);
        int get_fd();
        //int get_event_fd();

        void init_epoll();
        //void add_sockets_fd();
    private:
        //Epoll *epoll;
        Epoll epoll;
        SocketRepository *socket_repository;
        EventManager *event_manager;
        //t_epoll_event* get_event(int fd);

        //static EpollController& get_instance();
        //bool add(int fd, uint32_t event);
        //bool modify(int fd, uint32_t event);
        //bool erase(int fd);
        //int wait(t_epoll_event* event);
        //size_t get_fd_num();
        //int epfd();
        //bool get_error();
        //static EpollController* get_instance();

        //void erase_event(t_epoll_event* ev);
    private:
        //int epfd_;
        //static EpollController* _singleton;
        //bool _error;
        //std::map<int,  int> fd_table;
        //std::vector<t_epoll_event*> events;
        //bool check_error();
};

#endif
