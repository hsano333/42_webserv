#ifndef WEBSERV_EVENT_FACTORY_HPP
#define WEBSERV_EVENT_FACTORY_HPP
#include "socket_controller.hpp"
#include "socket_controller.hpp"
#include "webserv_event.hpp"
#include "event_manager.hpp"
#include "io_multiplexing.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"

class WebservEventFactory
{
    public:
        WebservEventFactory(
                SocketController *socket_controller,
                FDManager *fd_manager,
                IOMultiplexing *io_multi_controller,
                EventManager *event_manager,
                IWriter *normal_writer,
                IWriter *socket_writer,
                IReader *normal_reader,
                IReader *wocket_reader
                );
        ~WebservEventFactory();
        WebservEvent *from_epoll_event(t_epoll_event const &event);
        WebservEvent *make_application_event(WebservEvent *event);
        WebservEvent *make_write_event(WebservEvent *event, Response *res);
        WebservEvent *make_error_event(WebservEvent *event, StatusCode &code);
        WebservEvent *make_clean_event(WebservEvent *event);
        WebservEvent *make_read_event_from_event(WebservEvent *event);
    private:
        SocketController *socket_controller;
        FDManager *fd_manager;
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        IWriter *normal_writer;
        IWriter *socket_writer;
        IReader *normal_reader;
        IReader *socket_reader;
        

};

#endif
