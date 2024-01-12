#ifndef WEBSERV_IO_WORKER_HPP
#define WEBSERV_IO_WORKER_HPP

#include "webserv_event.hpp"
#include "webserv_read_event.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"
#include "socket_writer.hpp"
#include "socket_reader.hpp"

class WebservIOWorker
{
    public:
        //WebservReader();
        WebservIOWorker(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    SocketWriter *writer,
                    SocketReader *reader
                );
        ~WebservIOWorker();
        //void read(WebservReadEvent *event);
        void work(WebservEvent *event);
        //void recv_continue(WebservEvent *event);
        //bool have_executable_events();
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        SocketWriter *writer;
        SocketReader *reader;

};

#endif
