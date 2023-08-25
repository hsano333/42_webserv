#ifndef IO_MULTIPLEXING
#define IO_MULTIPLEXING

#include <climits>
#include "file_discriptor.hpp"

class IOMultiplexing
{
    public:
        virtual ~IOMultiplexing(){};
        virtual void wait() = 0;
        virtual void erase(FileDiscriptor fd) = 0;
        virtual void add(FileDiscriptor fd, uint32_t event) = 0;
        virtual void modify(FileDiscriptor fd, uint32_t event) = 0;
        virtual FileDiscriptor  get_fd() = 0;
        virtual bool have_executable_events() = 0;
        virtual std::vector<t_epoll_event> &take_out_event() = 0;
        //virtual std::vector<t_epoll_event> &get_event() = 0;
};

#endif
