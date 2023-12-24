#ifndef IO_MULTIPLEXING
#define IO_MULTIPLEXING

#include <climits>
#include <stdint.h>
#include "file_discriptor.hpp"
#include "epoll.hpp"

class IOMultiplexing
{
    public:
        virtual ~IOMultiplexing(){};
        virtual void wait(int msec) = 0;
        virtual void erase(FileDiscriptor fd) = 0;
        virtual void add(FileDiscriptor fd, uint32_t event) = 0;
        virtual void modify(FileDiscriptor fd, uint32_t event) = 0;
        virtual FileDiscriptor fd() = 0;
        //virtual bool have_executable_events() = 0;
        virtual std::vector<t_epoll_event> &take_out_event() = 0;
        virtual int executable_event_number() = 0;
};

#endif
