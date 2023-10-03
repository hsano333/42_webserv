#ifndef WEBSERV_EXECUTER_HPP
#define WEBSERV_EXECUTER_HPP
#include "webserv_event.hpp"
#include "event_manager.hpp"
#include "webserv_application_event.hpp"
#include "epoll_controller.hpp"

class WebservExecuter
{
    public:
        WebservExecuter(
                      IOMultiplexing *io_multi_controller,
                      EventManager *event_manager,
                      FDManager *fd_manager,
                      Config *cfg
                );
        ~WebservExecuter();
        void execute(WebservEvent *event);
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        FDManager *fd_manager;
        Config *cfg;
        void make_application(WebservApplicationEvent *event);
};

#endif
