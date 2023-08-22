#ifndef IO_MULTIPLEXING
#define IO_MULTIPLEXING

#include <climits>

class IOMultiplexing
{
    public:
        virtual ~IOMultiplexing(){};
        virtual void wait() = 0;
        virtual void erase(int fd) = 0;
        virtual void add(int fd, uint32_t event) = 0;
        virtual void modify(int fd, uint32_t event) = 0;
        virtual int  get_fd() = 0;
};

#endif
