#ifndef WEBSERV_H
#define WEBSERV_H
#include <climits>
#include <set>
#include <vector>
#include "config.hpp"
#include "tcp_socket.hpp"
#include "port.hpp"
#include "socket_manager.hpp"
#include "epoll_controller.hpp"
#include "webserv_waiter.hpp"
#include "webserv_event.hpp"
#include "webserv_receiver.hpp"
#include "webserv_parser.hpp"
#include "webserv_executer.hpp"
#include "webserv_sender.hpp"
#include "webserv_cleaner.hpp"
#include "event_manager.hpp"
#include "event_controller.hpp"

using std::map;
using std::string;
using std::vector;


class Webserv
{
  public:
    Webserv(Config *cfg,
            SocketManager *socket_manager,
            WebservEventFactory *event_factory,
            EventManager        *event_manager,
            EventController     *event_controller,
            //EpollController epoll_controller,
            WebservWaiter &waiter,
            WebservReceiver &receiver,
            WebservParser &parser,
            WebservExecuter &executer,
            WebservSender &sender,
            WebservCleaner &cleaner
            );
    Webserv(const std::vector<std::string> ports);
    //Webserv( c);
    Webserv(const Webserv& sockets);
    Webserv& operator=(const Webserv& sockets);
    ~Webserv();
    void communication();
    //bool change_epoll_config_to_write(FileDiscriptor fd, int event);
    void reset();

  private:
    Config              *cfg;
    SocketManager       *socket_manager;
    WebservEventFactory *event_factory;
    EventManager        *event_manager;
    EventController     *event_controller;
    //EpollController     &epoll_controller;
    WebservWaiter       &waiter;
    WebservReceiver     &receiver;
    WebservParser       &parser;
    WebservExecuter     &executer;
    WebservSender       &sender;
    WebservCleaner      &cleaner;
    IOMultiplexing      *io_multi_controller;

    void               wait_for_event();
    /*
    int _epfd;
    SocketManager *socket_manager;
    EpollController *epoll_controller;

    //void init_socket(std::set<Port>);
    bool init_epoll();
    void close_all();
    void connected_communication(FileDiscriptor fd, struct epoll_event* event, Socket* socket);
    vector<Socket*> _sockets;
    Socket* find_listen_socket(int socket_fd);
    const static unsigned int BODY_MAX = INT_MAX;
    map<int, Socket*> _fd_sockets;
    //map<int, CGI*> _fd_cgi;
    size_t count_fd();
    void timeout(int time_sec);
    //const Config* _config;
    void close_all_fd();
    */

};
#endif /* WEBSERV_H */
