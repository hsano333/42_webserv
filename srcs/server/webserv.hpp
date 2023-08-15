#ifndef WEBSERV_H
#define WEBSERV_H
#include "config.hpp"
#include "tcp_socket.hpp"
#include <climits>
#include <set>
#include <vector>
#include "port.hpp"

using std::map;
using std::string;
using std::vector;


class Webserv
{
  public:
    Webserv();
    Webserv(const std::vector<std::string> ports);
    //Webserv( c);
    Webserv(const Webserv& sockets);
    Webserv& operator=(const Webserv& sockets);
    ~Webserv();
    void communication();
    bool change_epoll_config_to_write(int fd, int event);
    void reset();

  private:
    int _epfd;
    void init_socket(std::set<Port>);
    bool init_epoll();
    void close_all();
    void connected_communication(int fd, struct epoll_event* event, Socket* socket);
    vector<Socket*> _sockets;
    Socket* find_listen_socket(int socket_fd);
    const static unsigned int BODY_MAX = INT_MAX;
    map<int, Socket*> _fd_sockets;
    //map<int, CGI*> _fd_cgi;
    size_t count_fd();
    void timeout(int time_sec);
    //const Config* _config;
    void close_all_fd();

};
#endif /* WEBSERV_H */
