#ifndef WEBSERV_RECEIVER_HPP
#define WEBSERV_RECEIVER_HPP

#include "webserv_event.hpp"
#include "webserv_read_event.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"
#include "socket_reader.hpp"

class WebservReceiver
{
    public:
        //WebservReader();
        WebservReceiver(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    SocketReader *reader
                );
        ~WebservReceiver();
        //void read(WebservReadEvent *event);
        void receiver(WebservEvent *event);
        //bool have_executable_events();
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        SocketReader *reader;

};

#endif
