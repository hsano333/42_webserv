#ifndef SOCKET_H
#define SOCKET_H
#include "config.hpp"
#include "fd_manager.hpp"
#include "request.hpp"
#include "request_cgi.hpp"
#include "response.hpp"
#include "response_cgi.hpp"
#include <iostream>
#include <netdb.h>
#include <string>
#include <sys/epoll.h>
#include <sys/socket.h>

#define BUF_MAX (1600)

typedef struct clientinfo {
    int fd;
    char buf[1024];
    int n;
    int state;
} s_clientinfo;

class Socket
{
  public:
    Socket();
    Socket(std::string port);
    Socket(const Socket& socket);
    Socket& operator=(const Socket& socket);
    virtual ~Socket();
    void close_fd();
    int getSockFD();
    Request* recv(int fd);
    bool send_response(Response* res, int fd);
    bool send_response(int fd);
    int accept_request();
    void set_response(int fd, Response* res);
    void set_response(int fd, ResponseCGI* res);
    void set_request(int fd, RequestCGI* req_cgi);
    void set_cgi(int fd, Response* res);
    Response* get_response(int fd);
    void erase_request(int fd);
    void erase_response(int fd);
    std::vector<int> timeout(int time);
    void erase_fd(int fd);
    void erase_all_fd();
    size_t count_fd();

  protected:
    int _sock_fd;
    const static int _SOCKET_NUM = 10;
    std::map<int, FDManager*> _fd_map;
    std::string _port;
    virtual void init();
    virtual int makeSocket();
    virtual void setAddrInfo(struct addrinfo& info);
    struct epoll_event _ev;
    s_clientinfo _clientinfo;
    bool send_err(std::string err);
    //Config const& _config;
    std::string _location_path;

    template <typename T>
    bool increment_timeout(T& obj, int time);
    bool _send_header;
    //const string crlf;
};

#include <iostream>
#include <stdio.h>
#include <string>

using std::cout;
using std::endl;
template <typename T>
bool Socket::increment_timeout(T& obj, int time)
{
    SocketData* socket_data = &obj;

    bool exceed;
    exceed = socket_data->increment_timeout(time);
    if (exceed) {
        return (true);
    }
    return (false);
}

#endif /* SOCKET_H */
