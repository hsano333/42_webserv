#ifndef WEBSERV_MAKER_HPP
#define WEBSERV_MAKER_HPP
#include "webserv_event.hpp"
#include "webserv_event_factory.hpp"
#include "event_manager.hpp"
#include "epoll_controller.hpp"

class WebservMaker
{
    public:
        //WebservMaker();
        WebservMaker(
                      IOMultiplexing *io_multi_controller,
                      //FDManager *fd_manager,
                      EventManager *event_manager,
                      WebservEventFactory *event_factory,
                      Config *cfg
                );
        ~WebservMaker();
        void make(WebservEvent *event);
    private:
        IOMultiplexing *io_multi_controller;
        //FDManager       *fd_manager;
        EventManager *event_manager;
        WebservEventFactory *event_factory;
        Config *cfg;
        bool check_body_size(Request *req, const ConfigServer *server);
        void parse_req(WebservEvent *event);
        void parse_res(WebservEvent *event);
};

#endif
