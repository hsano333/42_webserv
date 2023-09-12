#ifndef WEBSERV_CLEANER_HPP
#define WEBSERV_CLEANER_HPP
#include "io_multiplexing.hpp"
#include "event_manager.hpp"
#include "webserv_event.hpp"

class WebservCleaner
{
    public:
        WebservCleaner(
                        IOMultiplexing *io_multi_controller,
                        EventManager *event_manager,
                        FDManager *fd_manager

                );
        ~WebservCleaner();
        void clean(WebservEvent *event);
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        FDManager *fd_manager;

};
#endif
