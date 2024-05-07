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
#include "file_manager.hpp"
#include "event_manager.hpp"
#include "config.hpp"
#include "application_result.hpp"
#include "webserv_entity.hpp"
#include "webserv_cleaner.hpp"
#include "webserv_file_factory.hpp"

class EventManager;
class WebservEvent;
class ApplicationResult;
class WebservCleaner;
class WebservEventFactory
{
    public:
        WebservEventFactory(
                Config *cfg,
                WebservFileFactory *file_factory,
                SocketController *socket_controller,
                FDManager *fd_manager,
                FileManager *file_manager,
                IOMultiplexing *io_multi_controller,
                EventManager *event_manager,
                IWriter *normal_writer,
                IWriter *socket_writer,
                IReader *normal_reader,
                IReader *socket_reader,
                WebservCleaner *cleaner
                );
        ~WebservEventFactory();
        WebservEvent *from_epoll_event(t_epoll_event const &event);
        WebservEvent *make_io_socket_event_as_write(WebservEvent *event, WebservFile *src);
        WebservEvent *make_io_socket_event_as_read(WebservEvent *event);
        WebservEvent *make_io_socket_event_as_read(WebservEvent *event, WebservFile *src);
        WebservEvent *make_waiting_socket_out_cgi(WebservEvent *event);
        WebservEvent *make_waiting_out_cgi(WebservEvent *event);
        WebservEvent *make_waiting_out_cgi(WebservEvent *event, WebservFile *write_src, WebservFile *read_dst, ApplicationResult *result);
        WebservEvent *make_io_socket_for_cgi(WebservEvent *event);
        //WebservEvent *make_io_socket_for_cgi(WebservEvent *event, WebservFile *write_src, WebservFile *read_dst, ApplicationResult *result);
        void make_cgi_event(FileDiscriptor pid, FileDiscriptor fd_in, FileDiscriptor fd_out, Request *req);
        //void          register_file_manager(WebservEvent *event);
        WebservEvent *make_making_request_event(WebservEvent *event);
        WebservEvent *make_making_response_for_cgi_event(WebservEvent *event);
        WebservEvent *make_making_response_event(WebservEvent *event, WebservFile *src);
        WebservEvent *make_application_event(WebservEvent *event);
        WebservEvent *make_making_upload_event(WebservEvent *event, WebservFile *src);
        void          make_and_push_write_cgi_event(FileDiscriptor pid, FileDiscriptor fd_out, Request *req);
        WebservEvent *make_event_from_http_error(WebservEvent *event, char const *code);
        WebservEvent *make_clean_event(WebservEvent *event, bool force_close);
        WebservEvent *make_timeout_event(WebservEvent *event);
        WebservEvent *make_keep_alive_event(WebservEvent *event);
        WebservEvent *make_nothing_event(FileDiscriptor fd, FileDiscriptor sock_fd);
    private:
        Config *cfg;
        WebservFileFactory *file_factory;
        SocketController *socket_controller;
        FDManager *fd_manager;
        FileManager *file_manager;
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        IWriter *normal_writer;
        IWriter *socket_writer;
        IReader *normal_reader;
        IReader *socket_reader;
        WebservCleaner *cleaner;

};

#endif
