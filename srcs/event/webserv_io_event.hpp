#ifndef WEBSERV_IO_EVENT_HPP
#define WEBSERV_IO_EVENT_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"

class WebservIOEvent
{
    public:
        ~WebservIOEvent(){};
        virtual void switching_io(uint32_t epoll_event) = 0;
        virtual void set_write_io(File *src, File *dst) = 0;
        virtual void set_read_io(File *src, File *dst) = 0;
        virtual FileDiscriptor &get_write_fd() = 0;
        virtual FileDiscriptor &get_read_fd() = 0;
        virtual FileDiscriptor &get_socket_fd() = 0;

    private:
};

#endif
