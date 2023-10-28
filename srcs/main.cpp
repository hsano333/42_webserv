
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
#include "file.hpp"
#include "normal_file.hpp"
#include "normal_reader.hpp"
#include "stream_reader.hpp"
#include <fstream>
#include "body_size.hpp"
#include "ip_address.hpp"
#include "cidr.hpp"
#include "socket_factory.hpp"
#include "webserv_waiter.hpp"
#include "webserv_event.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_receiver.hpp"
#include "webserv_parser.hpp"
#include "webserv_executer.hpp"
#include "webserv_sender.hpp"
#include "event_manager.hpp"
#include "event_controller.hpp"
#include "fd_manager.hpp"
#include "ireader.hpp"
#include "normal_reader.hpp"
#include "socket_reader.hpp"
#include "normal_writer.hpp"
#include "socket_writer.hpp"
#include "iwriter.hpp"
#include "application_factory.hpp"
#include "cgi.hpp"

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


void clean_all(WebservCleaner &cleaner, EventManager *event_manager)
{
    event_manager->close_all_events_waiting_epoll(cleaner);
    //event_manager->close_all_events_waiting_writing(cleaner);
    event_manager->close_all_events();
    //cleaner.clean();
}

Config *create_config(std::string &cfg_file)
{
    File *file = NormalFile::from_filepath(cfg_file, std::ios::in);
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

    return (cfg_factory.create());
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

//boot_time = std::time(NULL);
#include <stdlib.h>
#include <sys/stat.h>
#include <iomanip>

       #include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>
int main(int argc, char const* argv[])
{
    /*
    std::string filepath = "/var/www/html/webserv/abc/cannot_delete_dir/d3";
    Utility::is_regular_file("/var/www/html/webserv/abc/cannot_delete_dir/d3");
    int rval = open(filepath.c_str(), O_RDONLY);
    if (rval){
        close(rval);
    }
    cout << "rval:" << rval << endl;
    */

    //exit(0);
    //std::exit(0);

    //int rval = remove(argv[1]);
    //cout << "rval:"  << rval << endl;
    //return 0;
    //boot_time = std::time(NULL);
    std::string abc = "abc";
    std::string def = "defghi";
    std::string ghi = "ghia";
    cout << std::setfill('1') << std::left << std::setw(12) << abc << def << endl;
    cout << std::setfill('1') << std::right<< std::setw(12) << abc << def << endl;
    cout << abc << std::setfill('1') << std::left  << std::setw(12) << def << endl;
    cout << abc << std::setfill('1') << std::right  << std::setw(12) << def << endl;
    cout << abc << std::setfill('1') << std::left  << std::setw(12) << ghi << endl;
    cout << abc << std::setfill('1') << std::right  << std::setw(12) << ghi << endl;
    //exit(0);

    std::string cfg_file = "./webserv.conf";
    if(argc > 2){
        return 0;
    }else if(argc == 2){
        cfg_file = argv[1];
    }

    Config *cfg;
    try{
        cfg = create_config(cfg_file);
    }catch(std::runtime_error &e){
        cout << "Config Error exit:" << e.what() << endl;
        exit(1);
    }

    //cfg->print_cfg();
    FDManager *fd_manager = new FDManager();
    SocketRepository *socket_repository = create_sockets(cfg, fd_manager);
    //Epoll epoll = Epoll::from_sockets(socket_repository);
    //
    cfg->check(socket_repository);


    //std::cout << epoll2.allocated_event_size() << endl;
    //exit(1);
    //Epoll *epoll = Epoll::from_socket_size(socket_manager->get_base_sockets_size());

    //Epoll *epoll = new Epoll();
    SocketController* socket_controller = new SocketController();
    EpollController *epoll_controller = new EpollController(Epoll(), socket_repository, socket_controller, fd_manager);
    epoll_controller->init_epoll();
    //epoll_controller.add_sockets_fd();
    (void)cfg;


    //Webserv
    //WebservDispatcher( wait(), map<Socket),epoll)
    //WebservReceiver(map<Socket),epoll)
    //WebservApplication(Socket)
    //WebservSender(Response,Socket)

    //        Config
    //        SocketRepository(map<fd,Socket>)
    //        SocketController(read,write)
    //        Epoll(epfd, vector<event>
    //        EpollController(add, modify, delete)
    //        ResponseRepository(map<fd, Response>)
    //        RequestRepository(map<fd, Request>)
    //        RequestReader(Socket)
    //        RequestParser(RequestReader)
    //        Request(RequestParser)
    //        EventManager
    //
    //        ResponseFactory(create(Request))
    //        ResponseSender(Socket)

    SocketReader *socket_reader = SocketReader::get_instance();
    NormalWriter *normal_writer = NormalWriter::get_instance();
    SocketWriter *socket_writer = SocketWriter::get_instance();
    NormalReader *normal_reader = NormalReader::get_instance();
    //StreamReader *stream_reader = StreamReader::get_instance();

    EventManager *event_manager = new EventManager();
    WebservEventFactory *event_factory = new WebservEventFactory(
            cfg,
            socket_controller,
            fd_manager,
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
            event_factory
            );

    CGI *cgi = new CGI();
    ApplicationFactory *application_factory = new ApplicationFactory(cfg, cgi);



    WebservWaiter waiter(epoll_controller, event_manager, event_factory);
    WebservReceiver reader(epoll_controller, event_manager, socket_reader);
    WebservParser parser(epoll_controller, event_manager, event_factory);
    WebservExecuter app(application_factory, epoll_controller, event_factory, event_manager, fd_manager, cfg);
    WebservSender sender(epoll_controller, fd_manager, event_factory, event_manager, socket_writer);
    WebservCleaner cleaner(epoll_controller, event_manager, fd_manager);

    SocketManager* socket_manager = new SocketManager();

    Webserv webserv(cfg,socket_manager,event_factory, event_manager, event_controller,waiter,reader,parser,app,sender, cleaner);
    while (1) {
        try{
            server(webserv);
            break;
        }catch(std::bad_alloc &e){
            WARNING("Webserv BalAlloc:");
            WARNING(e.what());
            clean_all(cleaner, event_manager);
        }catch(...){
            WARNING("Webserv Exception:");
            clean_all(cleaner, event_manager);
        }
    }
    cout << "end No.1" << endl;
    cout << "end No.1" << endl;
    cout << "end No.1" << endl;
    cout << "end No.1" << endl;
    cout << "end No.1" << endl;
    cout << "end No.1" << endl;
    cout << "end No.1" << endl;
    cout << "end No.1" << endl;
    clean_all(cleaner, event_manager);
    cout << "end No.2" << endl;
    delete fd_manager;
    delete socket_controller;
    delete epoll_controller;
    cout << "end No.3" << endl;
    delete event_manager;
    delete event_controller;
    delete socket_manager;
    cout << "end No.4" << endl;
    delete cfg;
    cout << "end No.5" << endl;
    delete event_factory;
    cout << "end No.6" << endl;
    delete (normal_writer);
    cout << "end No.7" << endl;
    delete (socket_writer);
    cout << "end No.8" << endl;
    delete (normal_reader);
    cout << "end No.9" << endl;
    delete (socket_reader);
    cout << "end No.10" << endl;
    cout << "end No.5" << endl;
    //delete cfg;
    cout << "end No.6" << endl;
    delete socket_repository;
    cout << "end No.7" << endl;
    delete application_factory;
    //delete stream_reader;
    delete cgi;
    DEBUG("end webserv");
    Log::delete_instance();
    cout << "end No.7" << endl;
    delete StreamReader::get_instance();
    cout << "end No.8" << endl;
    return 0;
}
