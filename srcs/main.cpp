
#include <stdlib.h>
#include <iostream>
#include <string>
#include "config.hpp"
#include "config_factory.hpp"
#include "config_raw_loader.hpp"
#include "iconfig_raw_getter.hpp"
#include "base64.hpp"
#include "split.hpp"
#include "utility.hpp"
#include "epoll_controller.hpp"
#include "webserv.hpp"
#include "global.hpp"
#include "endian.hpp"
#include "log.hpp"
#include "webserv_file.hpp"
#include "normal_file.hpp"
#include "normal_reader.hpp"
#include "stream_reader.hpp"
#include "stream_writer.hpp"
#include <fstream>
#include "body_size.hpp"
#include "ip_address.hpp"
#include "cidr.hpp"
#include "socket_factory.hpp"
#include "webserv_waiter.hpp"
#include "webserv_event.hpp"
#include "webserv_event_factory.hpp"
#include "event_manager.hpp"
#include "event_controller.hpp"
#include "fd_manager.hpp"
#include "file_manager.hpp"
#include "ireader.hpp"
#include "normal_reader.hpp"
#include "socket_reader.hpp"
#include "normal_writer.hpp"
#include "socket_writer.hpp"
#include "post_application.hpp"
#include "iwriter.hpp"
#include "application_factory.hpp"
#include "cgi.hpp"
#include "webserv_file_factory.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_waiting_post_cgi_event.hpp"
#include "webserv_application_event.hpp"
#include "webserv_application_upload_event.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_io_get_cgi_event.hpp"
#include "webserv_io_post_cgi_event.hpp"
#include "webserv_io_socket_event.hpp"
#include "webserv_keep_alive_event.hpp"
#include "webserv_make_request_event.hpp"
#include "webserv_make_response_event.hpp"
#include "webserv_make_response_for_get_cgi_event.hpp"
#include "webserv_make_response_for_post_cgi_event.hpp"
#include "webserv_nothing_event.hpp"
#include "webserv_timeout_event.hpp"
#include "webserv_waiting_get_cgi_event.hpp"
#include "webserv_waiting_post_cgi_event.hpp"
#include "delete_application.hpp"
#include "get_application.hpp"
#include "get_cgi_application.hpp"
#include "post_application.hpp"
#include "post_cgi_application.hpp"
#include "delete_application.hpp"
#include "webserv_error_event.hpp"
#include <stdlib.h>
#include <sys/stat.h>
#include <iomanip>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdint.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "uri.hpp"

#ifdef UNIT_TEST
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#endif



using std::cout;
using std::endl;
using std::string;
using std::vector;


#include <stdio.h>
__attribute__((destructor)) void f(void){
    system("leaks webserv");
}

std::string base_path = "srcs/dir";

void server(Webserv& webserv)
{
    try {
        webserv.communication();
    } catch (std::exception& e) {
        DEBUG("server error:" + string(e.what()));
        webserv.reset();
    }
}


void clean_all(WebservCleaner *cleaner, EventManager *event_manager, FDManager *fd_manager, FileManager *file_manager)
{
    std::map<FileDiscriptor, FileDiscriptor> fds = fd_manager->fd_sockets();
    std::map<FileDiscriptor, FileDiscriptor>::iterator ite = fds.begin();
    std::map<FileDiscriptor, FileDiscriptor>::iterator end = fds.end();

    while(ite != end){
        DEBUG("clean_all fd:" + ite->first.to_string());
        event_manager->erase_events_will_deleted_event(ite->first);
        file_manager->erase(ite->first);
        ite->first.close();
        ite++;
    }
    DEBUG("clean_all()");
    (void)cleaner;
    (void)event_manager;
}

Config *create_config(std::string &cfg_file, FDManager* fd_manager)
{
    NormalFile *file = NormalFile::from_filepath(cfg_file, std::ios::in);
    ConfigRawLoader raw_getter(file);
    ConfigParser<Config, ConfigHttp> parser_config("http");
    ConfigParser<ConfigHttp, ConfigServer> parser_http("server");
    ConfigParser<ConfigServer, ConfigLocation> parser_server("location");
    ConfigParser<ConfigLocation, ConfigLimit> parser_location("limit_except");
    ConfigParser<ConfigLimit, ConfigCgi> parser_limit("cgi");
    ConfigParser<ConfigCgi, ConfigLastObject> parser_cgi("");

    ConfigFactory cfg_factory = ConfigFactory(raw_getter,
                                              parser_config,
                                              parser_http,
                                              parser_server,
                                              parser_location,
                                              parser_limit,
                                              parser_cgi
    );

    return (cfg_factory.create(fd_manager));
}

SocketRepository *create_sockets(Config *cfg, FDManager *fd_manager)
{
    SocketFactory  factory = SocketFactory(fd_manager);
    SocketRepository* socket_repository = factory.create_from_config(cfg);
    return (socket_repository );
}


int main(int argc, char const* argv[])
{
    StatusCode::init_http_status_message();

    std::string cfg_file = "./webserv.conf";
    if(argc > 2){
        cout << "Invalid arguments" << endl;
        return 0;
    }else if(argc == 2){
        cout << "load Config file:" << argv[1] << endl;
        cfg_file = argv[1];
    }

    Config *cfg;
    FDManager *fd_manager = new FDManager();
    FileManager *file_manager = new FileManager();
    try{
        cfg = create_config(cfg_file, fd_manager);
    }catch(std::runtime_error &e){
        cout << "Webserv Config Error:" << e.what() << endl;
        exit(1);
    }catch(std::out_of_range &e){
        cout << "Webserv Config Error:" << e.what() << endl;
        exit(1);
    }catch(...){
        cout << "Webserv Config Error:" << endl;
        exit(1);
    }

    SocketRepository *socket_repository = create_sockets(cfg, fd_manager);
    cfg->check(socket_repository);
    SocketController* socket_controller = new SocketController();
    EpollController *epoll_controller = new EpollController(Epoll(), socket_repository, socket_controller, fd_manager);
    epoll_controller->init_epoll();
    (void)cfg;


    SocketReader *socket_reader = SocketReader::get_instance();
    NormalWriter *normal_writer = NormalWriter::get_instance();
    SocketWriter *socket_writer = SocketWriter::get_instance();
    NormalReader *normal_reader = NormalReader::get_instance();
    EventManager *event_manager = new EventManager();
    WebservFileFactory *file_factory = WebservFileFactory::get_instance(file_manager);

    PostApplication::get_instance(file_factory);

    WebservEventFactory *event_factory = new WebservEventFactory(
            cfg,
            file_factory,
            socket_controller,
            fd_manager,
            file_manager,
            epoll_controller,
            event_manager,
            normal_writer,
            socket_writer,
            normal_reader,
            socket_reader
            );
    EventController *event_controller = new EventController(
            event_manager,
            epoll_controller,
            event_factory,
            fd_manager
            );

    CGI *cgi = new CGI();
    ApplicationFactory *application_factory = ApplicationFactory::get_instance();
    application_factory->set_cgi(cgi);

    WebservWaiter waiter(epoll_controller, event_manager, event_factory);
    WebservCleaner *cleaner = new WebservCleaner(epoll_controller, event_manager, event_controller, fd_manager, file_manager);
    Webserv webserv(cfg, event_factory, event_manager, event_controller, waiter, cleaner);
    while (1) {
        try{
            server(webserv);
            break;
        }catch(std::bad_alloc &e){
            WARNING("Webserv BadAlloc:");
            WARNING(e.what());
            clean_all(cleaner, event_manager, fd_manager, file_manager);
        }catch(...){
            WARNING("Webserv Exception:");
            clean_all(cleaner, event_manager, fd_manager, file_manager);
        }
    }
    DEBUG("end server communication loop");
    clean_all(cleaner, event_manager, fd_manager, file_manager);


    delete ApplicationFactory::get_instance();
    delete DeleteApplication::get_instance();
    delete GetApplication::get_instance();
    delete GetCGIApplication::get_instance();
    delete PostApplication::get_instance();
    delete PostCGIApplication::get_instance();


    WebservApplicationEvent::delete_myself();
    WebservApplicationUploadEvent::delete_myself();
    WebservCleanEvent::delete_myself();
    WebservIOGetCGIEvent::delete_myself();
    WebservIOPostCGIEvent::delete_myself();
    WebservIOSocketEvent::delete_myself();
    WebservMakeRequestEvent::delete_myself();
    WebservMakeResponseEvent::delete_myself();
    WebservMakeResponseForGetCGIEvent::delete_myself();
    WebservMakeResponseForPostCGIEvent::delete_myself();
    WebservNothingEvent::delete_myself();
    WebservTimeoutEvent::delete_myself();
    WebservWaitingGetCGIEvent::delete_myself();
    WebservWaitingPostCGIEvent::delete_myself();
    WebservKeepAliveEvent::delete_myself();
    WebservErrorEvent::delete_myself();

    delete WebservFileFactory::get_instance();
    delete NormalReader::get_instance();
    delete NormalWriter::get_instance();
    delete SocketReader::get_instance();
    delete SocketWriter::get_instance();
    delete StreamReader::get_instance();
    delete StreamWriter::get_instance();
    delete Endian::get_instance();


    delete fd_manager;
    delete file_manager;
    delete socket_controller;
    delete epoll_controller;
    delete event_manager;
    delete event_controller;
    delete cfg;
    delete event_factory;
    delete socket_repository;
    delete cgi;
    delete cleaner;
    Log::delete_instance();

    return 0;
}
