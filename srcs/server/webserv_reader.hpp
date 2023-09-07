#ifndef WEBSERV_READER_HPP
#define WEBSERV_READER_HPP

#include "webserv_event.hpp"
#include "webserv_read_event.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"

class WebservReader
{
    public:
        //WebservReader();
        WebservReader(IOMultiplexing *io_multi_controller,
                      EventManager *event_manager
                );
        ~WebservReader();
        //void read(WebservReadEvent *event);
        void read(WebservEvent *event);
        //bool have_executable_events();
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;

};

#endif
