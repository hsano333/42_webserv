#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP
#include "webserv_event.hpp"
#include "webserv_event_factory.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"

class WebservParser
{
    public:
        //WebservParser();
        WebservParser(IOMultiplexing *io_multi_controller,
                      EventManager *event_manager,
                      WebservEventFactory *event_factory
                );
        ~WebservParser();
        void parse_req(WebservEvent *event);
        void parse_res(WebservEvent *event);
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        WebservEventFactory *event_factory;
};

#endif
