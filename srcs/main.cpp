
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
#include "iwriter.hpp"
#include "application_factory.hpp"
#include "cgi.hpp"
#include "webserv_file_factory.hpp"

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
        webserv.reset();
    }
}

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "uri.hpp"


void clean_all(WebservCleaner *cleaner, EventManager *event_manager)
{
    event_manager->close_all_events_waiting_epoll(cleaner);
    //event_manager->close_all_events_waiting_writing(cleaner);
    event_manager->close_all_events();
    //cleaner.clean();
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

    //return (cfg_factory.create(fd_manager));
    return (cfg_factory.create(fd_manager));
}


/*
SocketManager *create_socket_manager(Config *cfg)
{
    SocketFactory  factory = SocketFactory();
    //SocketManager* socket_manager = factory.create(cfg);
    //return (socket_manager);
    SocketRepository* socket_repository = factory.create_from_config(cfg);
    return (socket_repository );
}
*/
SocketRepository *create_sockets(Config *cfg, FDManager *fd_manager)
{
    SocketFactory  factory = SocketFactory(fd_manager);
    //SocketManager* socket_manager = factory.create(cfg);
    //return (socket_manager);
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
int main(int argc, char const* argv[])
{

    CGI *cgi1 = new CGI();
    CGI *cgi2 = new CGI();
    CGI *cgi3 = new CGI();
    CGI *cgi4 = new CGI();
    CGI *cgi5 = new CGI();

    std::set<CGI*> cgi_set;
    cgi_set.insert(cgi1);
    cgi_set.insert(cgi2);
    cgi_set.insert(cgi3);
    cgi_set.insert(cgi4);
    cgi_set.insert(cgi5);

    std::set<CGI*>::iterator ite = cgi_set.begin();
    std::set<CGI*>::iterator end = cgi_set.end();

    while(ite != end){

        delete *ite;
        ite++;
    }
    cout << "end" << endl;
    //exit(0);

    std::string cfg_file = "./webserv.conf";
    if(argc > 2){
        return 0;
    }else if(argc == 2){
        cfg_file = argv[1];
    }

    Config *cfg;
    FDManager *fd_manager = new FDManager();
    FileManager *file_manager = new FileManager();
    try{
        cfg = create_config(cfg_file, fd_manager);
    }catch(std::runtime_error &e){
        cout << "Config Error exit:" << e.what() << endl;
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
    //StreamReader *stream_reader = StreamReader::get_instance();
    EventManager *event_manager = new EventManager();
    WebservCleaner *cleaner = new WebservCleaner(epoll_controller, event_manager, fd_manager, file_manager);
    WebservFileFactory *file_factory = WebservFileFactory::get_instance(file_manager);

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
    clean_all(cleaner, event_manager);
    delete fd_manager;
    delete file_manager;
    delete socket_controller;
    delete epoll_controller;
    delete event_manager;
    delete event_controller;
    //delete socket_manager;
    delete cfg;
    delete event_factory;
    delete (normal_writer);
    delete (socket_writer);
    delete (normal_reader);
    delete (socket_reader);
    DEBUG("delete No.1");
    delete socket_repository;
    DEBUG("delete No.2");
    //delete application_factory;
    DEBUG("delete No.3");
    //delete stream_reader;
    delete cgi;
    DEBUG("delete No.4");
    Log::delete_instance();
    DEBUG("delete No.5");
    delete StreamReader::get_instance();
    DEBUG("delete No.6");
    delete StreamWriter::get_instance();
    DEBUG("end webserv");
    delete cleaner;
    return 0;
}
