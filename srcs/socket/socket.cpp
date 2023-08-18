// #include "sock_fdet.hpp"
#include "config.hpp"
#include "response_cgi.hpp"
#include "string.h"
#include "tcp_socket.hpp"
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>

using std::cout;
using std::endl;
using std::cerr;

int Socket::makeSocket()
{
    return (socket(AF_INET, SOCK_STREAM, 0));
}

void Socket::setAddrInfo(struct addrinfo& info)
{
    info.ai_family = AF_INET;
    info.ai_flags = AI_PASSIVE;
    info.ai_socktype = SOCK_STREAM;
}

void Socket::init()
{
    this->_sock_fd = makeSocket();
    memset(&(this->_clientinfo), 0, sizeof(s_clientinfo));
    this->_ev.data.ptr = &_clientinfo;
    if (this->_sock_fd < 0) {
        cout << strerror(errno) << endl;
        throw std::runtime_error("Failed to create sock_fdet\n");
    }

    struct addrinfo hint;
    memset(&hint, 0, sizeof(struct addrinfo));
    setAddrInfo(hint);
    struct addrinfo* res = NULL;
    int err = getaddrinfo(NULL, this->port.c_str(), &hint, &res);
    if (err != 0) {
        this->close_fd();
        cout << "Error getaddrinfo() :" << gai_strerror(err) << endl;
        throw std::runtime_error("Failed to init()\n");
    }
    int yes = 1;
    setsockopt(this->_sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&yes, sizeof(yes));
    if (bind(this->_sock_fd, res->ai_addr, res->ai_addrlen) != 0) {
        this->close_fd();
        freeaddrinfo(res);
        cout << "Error bind1:" << strerror(errno) << endl;
        throw std::runtime_error("Failed to bind()\n");
    }
    freeaddrinfo(res);
    listen(this->_sock_fd, _SOCKET_NUM);
}

Socket::Socket() : _sock_fd(0),  _send_header(false)
{
    //this->port = Port();
    this->port = Port::from_int(11111);
    init();
}

Socket::Socket(Port &port)
    : _sock_fd(0),
      port(port),
      _send_header(false)
{
    init();
}

Socket::Socket(Port const &port)
    : _sock_fd(0),
      port(port),
      _send_header(false)
{
    init();
}

Socket::~Socket()
{
}
Socket::Socket(const Socket& sock_fdet)
    : _sock_fd(sock_fdet._sock_fd),
      port(sock_fdet.port)
{
    init();
}
Socket& Socket::operator=(const Socket& sock_fdet)
{
    if (&sock_fdet == this)
        return (*this);
    return (*this);
}

int Socket::getSockFD()
{
    return (this->_sock_fd);
}

void Socket::close_fd()
{
    cerr << "Socket close:" << _sock_fd << endl;
    close(this->_sock_fd);
}

int Socket::accept_request()
{
    struct sockaddr_in client;
    memset(&client, 0, sizeof(struct sockaddr_in));
    socklen_t len = sizeof(client);

    int fd = accept(this->_sock_fd, (struct sockaddr*)&client, &len);
    if (fd < 0) {
        cout << "Error accept():" << strerror(errno) << endl;
        // return ();
    }

    FDManager* fd_m = new FDManager(fd);
    this->_fd_map.insert(std::make_pair(fd, fd_m));
    int cur_flags = fcntl(fd, F_GETFL, 0);
    cur_flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, cur_flags);
    return (fd);
}

Request* Socket::recv(int fd)
{
    Request* req = this->_fd_map[fd]->get_req();
    if (req != NULL){
        return req;
    }
    // try {
    // req = new Request(fd, _config);
    req = new Request(fd);
    req->set_port(port);
    this->_fd_map[fd]->insert(req);
    // } catch (std::exception& e) {
    //     req = NULL;
    //     this->_fd_map[fd]->insert(req);
    //     cout << e.what() << endl;
    // }
    return (req);
}

bool Socket::send_response(int fd)
{
    char data[BUF_MAX + 100];
    char *bk = &(data[0]);
    char* p_data = &(data[0]);
    int size;
    Response* res = get_response(fd);
    int open_result = res->open_responsed_file();
    size = res->get_data(&(p_data));
    string crlf = "\r\n";

    if (open_result == false){
        string err = res->get_error();
        int send1 = send(fd, err.c_str(), err.size(), MSG_DONTWAIT);
        if (send1 < 0){
            return (false);
        }
        return (true);
    }
    while (open_result && size > 0) {
        //int ii=0;
        /*
        while(ii < size){
            printf("%c", p_data[ii]);
            ii++;
        }
        */
        int send1 = send(fd, p_data, size, MSG_DONTWAIT);
        p_data[size] = '\0';
        cout << "p_data[" << p_data << "]" << endl;
        int send2 = send(fd, crlf.c_str(), crlf.size(), MSG_DONTWAIT);
        if (send1 < 0 || send2 < 0){
            break;
        }
        p_data = bk;
        size = res->get_data(&(p_data));
    }
    std::string last = "0\r\n";
    send(fd, last.c_str(), last.size(), MSG_DONTWAIT);
    send(fd, crlf.c_str(), crlf.size(), MSG_DONTWAIT);

    return (true);
}


void Socket::set_request(int fd, RequestCGI* req_cgi)
{
    FDManager* fd_m = new FDManager(fd);
    this->_fd_map.insert(std::make_pair(fd, fd_m));
    this->_fd_map[fd]->insert(req_cgi);

    //this->set_response(fd, static_cast<Request*>(res_cgi));
    cout << "set_response CGI Req No.2" << endl;
}

void Socket::set_response(int fd, ResponseCGI* res_cgi)
{
    //Response *res = res_cgi;
    //FDManager* fd_m = static_cast<FDManager *>( static_cast< Response*>(res_cgi));
    cout << "set_response CGI No.1 fd=" << fd << endl;

    FDManager* fd_m = new FDManager(fd);
    this->_fd_map.insert(std::make_pair(fd, fd_m));
    this->set_response(fd, static_cast<Response*>(res_cgi));
    cout << "set_response CGI No.2" << endl;
}

void Socket::set_response(int fd, Response* res)
{
    cout << "set_response No.1 fd=" << fd << endl;
    this->_fd_map[fd]->insert(res);
    cout << "set_response No.2" << endl;
}

Response* Socket::get_response(int fd)
{
    return (this->_fd_map[fd]->get_res());
}

std::vector<int> Socket::timeout(int time)
{
    std::map<int, FDManager*>::iterator iter = _fd_map.begin();
    std::map<int, FDManager*>::iterator end = _fd_map.end();
    std::vector<int> delete_fd;

    FDManager* fd_m;
    int fd;
    bool timeout;
    for (; iter != end; iter++) {
        fd = iter->first;
        fd_m = iter->second;
        if (fd_m->get_req()) {
            timeout = increment_timeout(*fd_m->get_req(), time);
        } else if (fd_m->get_res()) {
            timeout = increment_timeout(*fd_m->get_res(), time);
        } else {
            timeout = increment_timeout(*fd_m, time);
        }
        if (timeout) {
            delete_fd.push_back(fd);
        }
    }
    return (delete_fd);
}

size_t Socket::count_fd()
{
    return (this->_fd_map.size());
}

void Socket::erase_request(int fd)
{
    this->_fd_map[fd]->delete_req();
}

void Socket::erase_response(int fd)
{
    // Response *tmp = _fd_map[fd].get_res();
    // delete tmp;
    this->_fd_map[fd]->delete_res();
}

void Socket::erase_fd(int fd)
{
    FDManager* fd_m = this->_fd_map[fd];
    fd_m->close_fd();
    delete fd_m;
    this->_fd_map.erase(fd);
}

void Socket::erase_all_fd()
{
    map<int, FDManager*>::iterator ite = _fd_map.begin();
    map<int, FDManager*>::iterator end = _fd_map.end();

    for(; ite != end; ite++){
        FDManager* fd_m = ite->second;
        fd_m->close_fd();
        delete fd_m;
    }
    this->_fd_map.clear();
}
