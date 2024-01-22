#ifndef WEBSERV_IO_EVENT_HPP
#define WEBSERV_IO_EVENT_HPP
#include "webserv_event.hpp"

class WebservIOEvent
{
    public:
        ~WebservIOEvent(){};
        virtual void set_io(uint32_t epoll_event) = 0;

    private:
};

#endif
