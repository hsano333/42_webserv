#ifndef WEBSERV_EXECUTER_HPP
#define WEBSERV_EXECUTER_HPP
#include "webserv_event.hpp"
#include "event_manager.hpp"
#include "webserv_application_event.hpp"
#include "epoll_controller.hpp"
#include "application_factory.hpp"
#include "webserv_event_factory.hpp"

class WebservExecuter
{
    public:
        WebservExecuter(
                      ApplicationFactory *factory,
                      IOMultiplexing *io_multi_controller,
                      WebservEventFactory *event_factory,
                      EventManager *event_manager,
                      FDManager *fd_manager,
                      Config *cfg
                );
        ~WebservExecuter();
        void execute(WebservEvent *event);
    private:
        ApplicationFactory *factory;
        IOMultiplexing *io_multi_controller;
        WebservEventFactory *event_factory;
        EventManager *event_manager;
        FDManager *fd_manager;
        Config *cfg;
        Application *get_application(WebservApplicationEvent *event);
};

#endif
