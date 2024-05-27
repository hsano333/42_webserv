
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
//#include "normal_webserv_file.hpp"
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
//#include "webserv_waiting_cgi_out_event.hpp"
#include "webserv_waiting_get_cgi_event.hpp"
#include "webserv_waiting_post_cgi_event.hpp"
//#include "directory_reader.hpp"
//#include "socket_reader_chunked.hpp"
#include "delete_application.hpp"
#include "get_application.hpp"
#include "get_cgi_application.hpp"
#include "post_application.hpp"
#include "post_cgi_application.hpp"
#include "delete_application.hpp"
#include "webserv_error_event.hpp"

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


// std::string base_path = "/home/sano/work/42/webserv/github/webserv/srcs/dir";
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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "uri.hpp"


void clean_all(WebservCleaner *cleaner, EventManager *event_manager)
{
    DEBUG("clean_all()");
    (void)cleaner;
    (void)event_manager;
    //event_manager->close_all_events_waiting_epoll(cleaner);
    //event_manager->close_all_events();
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

#include <stdlib.h>
#include <sys/stat.h>
#include <iomanip>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdint.h>
#include <stdlib.h>

int main(int argc, char const* argv[])
{

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
        cout << "Config Error:" << e.what() << endl;
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
    WebservCleaner *cleaner = new WebservCleaner(epoll_controller, event_manager, fd_manager, file_manager);
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
            socket_reader,
            cleaner
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
    Webserv webserv(cfg, event_factory, event_manager, event_controller, waiter, cleaner);
    while (1) {
        try{
            server(webserv);
            DEBUG("end server communication");
            break;
        }catch(std::bad_alloc &e){
            WARNING("Webserv BadAlloc:");
            WARNING(e.what());
            clean_all(cleaner, event_manager);
        }catch(...){
            WARNING("Webserv Exception:");
            clean_all(cleaner, event_manager);
        }
    }
    DEBUG("end server communication loop");
    clean_all(cleaner, event_manager);


    delete ApplicationFactory::get_instance();
    delete DeleteApplication::get_instance();
    delete GetApplication::get_instance();
    delete GetCGIApplication::get_instance();
    delete PostApplication::get_instance();
    delete PostCGIApplication::get_instance();


    /*

    delete DirectoryReader::get_instance();
    delete NormalReader::get_instance();
    delete NormalWriter::get_instance();
    delete SocketReader::get_instance();
    delete SocketReaderChunked::get_instance();
    delete SocketWriter::get_instance();
    delete StreamReader::get_instance();
    delete StreamWriter::get_instance();
    delete WebservFileFactory::get_instance();
    delete Endian::get_instance();


    delete WebservApplicationEvent::get_instance();
    delete WebservApplicationUploadEvent::get_instance();
    delete WebservCleanEvent::get_instance();
    delete WebservIOGetCGIEvent::get_instance();
    delete WebservIOPostCGIEvent::get_instance();
    delete WebservIOSocketEvent::get_instance();
    delete WebservKeepAliveEvent::get_instance();
    delete WebservMakeRequestEvent::get_instance();
    delete WebservMakeResponseEvent::get_instance();
    delete WebservMakeResponseForGetCGIEvent::get_instance();
    delete WebservMakeResponseForPostCGIEvent::get_instance();
    delete WebservNothingEvent::get_instance();
    delete WebservTimeoutEvent::get_instance();
    delete WebservWaitingGetCGIEvent::get_instance();
    delete WebservWaitingPostCGIEvent::get_instance();
    */
    DEBUG("end server communication loop No.1");
    WebservApplicationEvent::delete_myself();
    WebservApplicationUploadEvent::delete_myself();
    WebservCleanEvent::delete_myself();
    DEBUG("end server communication loop No.12");
    WebservIOGetCGIEvent::delete_myself();
    WebservIOPostCGIEvent::delete_myself();
    WebservIOSocketEvent::delete_myself();
    DEBUG("end server communication loop No.13");
    WebservMakeRequestEvent::delete_myself();
    WebservMakeResponseEvent::delete_myself();
    WebservMakeResponseForGetCGIEvent::delete_myself();
    WebservMakeResponseForPostCGIEvent::delete_myself();
    WebservNothingEvent::delete_myself();
    WebservTimeoutEvent::delete_myself();
    //WebservWaitingCGIOUTEvent::delete_myself();
    DEBUG("end server communication loop No.14");
    WebservWaitingGetCGIEvent::delete_myself();
    WebservWaitingPostCGIEvent::delete_myself();
    WebservKeepAliveEvent::delete_myself();
    DEBUG("end server communication loop No.15");
    WebservErrorEvent::delete_myself();
    DEBUG("end server communication loop No.2");

    //delete DirectoryReader::get_instance();
    delete WebservFileFactory::get_instance();
    delete NormalReader::get_instance();
    delete NormalWriter::get_instance();
    delete SocketReader::get_instance();
    //delete SocketReaderChunked::get_instance();
    delete SocketWriter::get_instance();
    delete StreamReader::get_instance();
    delete StreamWriter::get_instance();
    delete Endian::get_instance();


    DEBUG("end server communication loop No.3");

    delete fd_manager;
    delete file_manager;
    delete socket_controller;
    delete epoll_controller;
    delete event_manager;
    delete event_controller;
    //delete socket_manager;
    delete cfg;
    delete event_factory;
    //delete (normal_writer);
    //delete (socket_writer);
    //delete (normal_reader);
    //delete (socket_reader);
    DEBUG("delete No.1");
    delete socket_repository;
    DEBUG("delete No.2");
    //delete application_factory;
    DEBUG("delete No.3");
    //delete stream_reader;
    delete cgi;
    DEBUG("delete No.4");
    DEBUG("delete No.5");
    DEBUG("delete No.6");
    DEBUG("end webserv");
    delete cleaner;
    Log::delete_instance();



    return 0;
}
