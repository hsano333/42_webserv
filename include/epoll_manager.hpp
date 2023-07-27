#ifndef EPOLL_MANAGER_HPP
#define EPOLL_MANAGER_HPP

#include <vector>
#include <map>
#include <sys/epoll.h>

typedef struct epoll_event t_epoll_event;
class EpollManager
{
    public:
        ~EpollManager();
        //static EpollManager& get_instance();
        int _epfd;
        bool add(int fd, uint32_t event);
        bool modify(int fd, uint32_t event);
        bool erase(int fd);
        int wait(t_epoll_event* event);
        bool get_error();
        static EpollManager* get_instance();
        size_t get_fd_num();
    private:
        EpollManager();
        static EpollManager* _singleton;
        bool _error;
        std::map<int,  int> fd_table;
        std::vector<struct epoll_event>::iterator get_event(int fd);
        std::vector<struct epoll_event> _events;
        bool check_error();
};

#endif
