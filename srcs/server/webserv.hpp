#ifndef WEBSERV_HPP
#define WEBSERV_HPP
#include <climits>
#include <set>
#include <vector>
#include "config.hpp"
#include "tcp_socket.hpp"
#include "port.hpp"
#include "epoll_controller.hpp"
#include "webserv_waiter.hpp"
#include "webserv_event.hpp"
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
            WebservEventFactory *event_factory,
            EventManager        *event_manager,
            EventController     *event_controller,
            WebservWaiter       &waiter,
            WebservCleaner      &cleaner
            );
    Webserv(const std::vector<std::string> ports);
    Webserv(const Webserv& sockets);
    Webserv& operator=(const Webserv& sockets);
    ~Webserv();
    void communication();
    void reset();

  private:
    Config              *cfg;
    WebservEventFactory *event_factory;
    EventManager        *event_manager;
    EventController     *event_controller;
    WebservWaiter       &waiter;
    WebservCleaner      &cleaner;
    IOMultiplexing      *io_multi_controller;

    void               wait_for_event();
};
#endif /* WEBSERV_H */
