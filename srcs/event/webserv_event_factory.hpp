#ifndef WEBSERV_EVENT_FACTORY_HPP
#define WEBSERV_EVENT_FACTORY_HPP
#include "socket_controller.hpp"
#include "socket_controller.hpp"
#include "webserv_event.hpp"
#include "event_manager.hpp"
#include "io_multiplexing.hpp"
#include "ireader.hpp"
#include "iwriter.hpp"
#include "fd_manager.hpp"
#include "event_manager.hpp"
#include "config.hpp"

class EventManager;
class WebservEvent;
class WebservEventFactory
{
    public:
        WebservEventFactory(
                Config *cfg,
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
        void make_cgi_event(FileDiscriptor pid, FileDiscriptor fd_in, FileDiscriptor fd_out, Request *req);
        WebservEvent *make_making_request_event(WebservEvent *event);
        WebservEvent *make_application_event(WebservEvent *event);
        void          make_and_push_write_cgi_event(FileDiscriptor pid, FileDiscriptor fd_out, Request *req);
        void          make_and_push_read_cgi_event(FileDiscriptor pid, FileDiscriptor fd_in);
        WebservEvent *make_write_event_for_cgi(WebservEvent *event, Response *res);
        WebservEvent *make_write_event(WebservEvent *event, Response *res);
        WebservEvent *make_error_event(WebservEvent *event, char const *code);
        WebservEvent *make_clean_event(WebservEvent *event, bool force_close);
        WebservEvent *make_read_event_from_event(WebservEvent *event);
        WebservEvent *make_nothing_event(FileDiscriptor fd);
        WebservEvent *make_nothing_event(FileDiscriptor fd, FileDiscriptor sock_fd);
    private:
        Config *cfg;
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
