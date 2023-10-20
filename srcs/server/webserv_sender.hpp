#ifndef WEBSERV_SENDER_HPP
#define WEBSERV_SENDER_HPP
#include "webserv_event.hpp"
#include "webserv_event_factory.hpp"
#include "io_multiplexing.hpp"
#include "event_manager.hpp"
#include "socket_writer.hpp"
class WebservSender
{
    public:
        WebservSender(
                    IOMultiplexing *io_multi_controller,
                    FDManager *fd_manager,
                    WebservEventFactory *event_factory,
                    EventManager *event_manager,
                    SocketWriter *writer
                );
        ~WebservSender();
        void send(WebservEvent *event);
    private:
        IOMultiplexing  *io_multi_controller;
        FDManager *fd_manager;
        WebservEventFactory *event_factory;
        EventManager    *event_manager;
        SocketWriter *writer;

};

#endif
