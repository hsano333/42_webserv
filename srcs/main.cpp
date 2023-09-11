
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
#include "read_raw.hpp"
#include <fstream>
#include "body_size.hpp"
#include "ip_address.hpp"
#include "cidr.hpp"
#include "socket_factory.hpp"
#include "webserv_waiter.hpp"
#include "webserv_event.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_reader.hpp"
#include "webserv_parser.hpp"
#include "webserv_application.hpp"
#include "webserv_sender.hpp"
#include "event_manager.hpp"
#include "fd_manager.hpp"

#ifdef UNIT_TEST
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN 
#include "doctest.h"
#endif



using std::cout;
using std::endl;
using std::string;
using std::vector;

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

Config *create_config(std::string &cfg_file)
{
    ReadRaw read_raw;
    File file = File(read_raw, cfg_file, READ_ONLY);
    ConfigRawLoader raw_getter(file);
    ConfigParser<Config, ConfigHttp> parser_config("http");
    ConfigParser<ConfigHttp, ConfigServer> parser_http("server");
    ConfigParser<ConfigServer, ConfigLocation> parser_server("location");
    ConfigParser<ConfigLocation, ConfigLimit> parser_location("limit_except");
    ConfigParser<ConfigLimit, ConfigLastObject> parser_limit("");

    ConfigFactory cfg_factory = ConfigFactory(raw_getter,
                                              parser_config,
                                              parser_http,
                                              parser_server,
                                              parser_location,
                                              parser_limit
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

int main(int argc, char const* argv[])
{
    std::string cfg_file = "./webserv.conf";
    if(argc > 2){
        return 0;
    }else if(argc == 2){
        cfg_file = argv[1];
    }

    Config *cfg = create_config(cfg_file);

    std::cout << "made config " << std::endl;
    //cfg->print_cfg();
    FDManager *fd_manager = new FDManager();
    SocketRepository *socket_repository = create_sockets(cfg, fd_manager);
    //Epoll epoll = Epoll::from_sockets(socket_repository);


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

    EventManager *event_manager = new EventManager();
    WebservEventFactory *event_factory = new WebservEventFactory(socket_controller, fd_manager, epoll_controller);
    WebservWaiter waiter(epoll_controller, event_manager, event_factory);
    WebservReader reader(epoll_controller, event_manager);
    WebservParser parser(epoll_controller, event_manager, event_factory);
    WebservApplication app(epoll_controller, event_manager, fd_manager);
    WebservSender sender(epoll_controller, fd_manager);

    SocketManager* socket_manager = new SocketManager();

    Webserv webserv(cfg,socket_manager,waiter,reader,parser,app,sender);
    //while (1) {
        server(webserv);
    //}
    delete fd_manager;
    delete socket_controller;
    delete epoll_controller;
    delete event_manager;
    delete event_factory;
    delete socket_manager;
    delete cfg;
    delete socket_repository;
    return 0;
}
