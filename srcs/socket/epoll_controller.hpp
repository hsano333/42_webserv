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
        void add(FileDiscriptor fd, uint32_t event);
        void erase(FileDiscriptor fd);
        void modify(FileDiscriptor fd, uint32_t event);
        FileDiscriptor get_fd();
        bool have_executable_events();
        std::vector<t_epoll_event> &take_out_event();
        //int get_
        //int get_event_fd();

        void init_epoll();
        //void add_sockets_fd();
    private:
        //Epoll *epoll;
        Epoll epoll;
        SocketRepository *socket_repository;
        EventManager *event_manager;
        //t_epoll_event* get_event(FileDiscriptor fd);

        //static EpollController& get_instance();
        //bool add(FileDiscriptor fd, uint32_t event);
        //bool modify(FileDiscriptor fd, uint32_t event);
        //bool erase(FileDiscriptor fd);
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
