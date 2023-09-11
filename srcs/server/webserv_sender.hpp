#ifndef WEBSERV_SENDER_HPP
#define WEBSERV_SENDER_HPP
#include "webserv_event.hpp"
#include "io_multiplexing.hpp"
#include "event_manager.hpp"
class WebservSender
{
    public:
        WebservSender(
                    IOMultiplexing *io_multi_controller,
                    FDManager *fd_manager
                    //EventManager *event_manager
                );
        ~WebservSender();
        void send(WebservEvent *event);
    private:
        IOMultiplexing  *io_multi_controller;
        FDManager *fd_manager;
        //EventManager    *event_manager;

};

#endif
