
#include "webserv.hpp"

#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "cgi.hpp"
//#include "file_utility.hpp"
#include "request.hpp"
#include "response.hpp"
#include "response_cgi.hpp"
#include "tcp_socket.hpp"
#include "utility.hpp"
#include "log.hpp"
#include "global.hpp"
#include "epoll_controller.hpp"

#define NEVENTS 16
using std::cout;
using std::endl;
using std::string;

/*
Webserv::Webserv() : epfd(0)
{
    std::vector<std::string> vec;
    vec.push_back("80");
    init_socket(vec);
    _config = Config();
}

Webserv::Webserv(const std::vector<std::string> ports)epfd(0)
{
    init_socket(ports);
}
*/

Webserv::Webserv(
        Config *cfg,
        SocketManager *socket_manager,
        //EpollController epoll_controller,
        WebservWaiter &waiter,
        WebservReader &reader,
        WebservParser &parser,
        WebservExecuter &executer,
        WebservSender &sender,
        WebservCleaner &cleaner
        ) :
                     //_epfd(0),
                     cfg(cfg),
                     socket_manager(socket_manager),
                     //epoll_controller(epoll_controller),
                     waiter(waiter),
                     reader(reader),
                     parser(parser),
                     executer(executer),
                     sender(sender),
                     cleaner(cleaner)
{
    ;
}

Webserv::~Webserv()
{
    //close_all();
}
/*
Webserv::Webserv(const Webserv &webserv) : epfd(0)
{
    (void)webserv;
}
*/
Webserv& Webserv::operator=(const Webserv& socket)
{
    if (&socket == this)
        return (*this);
    return (*this);
}



/*
void Webserv::close_all()
{
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        this->_sockets[i]->close_fd();
    }
}
*/

//bool Webserv::init_epoll()
//{
    /*
    EpollController* epoll_controller = EpollController::get_instance();
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        Socket* socket = this->_sockets[i];
        //ev.events = EPOLLIN;
        int fd = socket->get_socket_fd();
        epoll_controller->add(fd, EPOLLIN);
    }
    this->_epfd = epoll_create(this->_sockets.size());
    if (_epfd < 0) {
        cout << "Epoll Error:" << strerror(errno) << endl;
        return (false);
    }
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        struct epoll_event ev;
        memset(&ev, 0, sizeof(struct epoll_event));
        Socket* socket = this->_sockets[i];

        ev.events = EPOLLIN;
        ev.data.fd = socket->get_socket_fd();
        if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, socket->get_socket_fd(), &ev) != 0) {
            cout << "Epoll Ctl Error:" << strerror(errno) << endl;
            cout << "Epoll Ctl Error:" << strerror(errno) << endl;
            cout << "Epoll Ctl Error:" << strerror(errno) << endl;
            cout << "Epoll Ctl Error:" << strerror(errno) << endl;
            return (false);
        }
    }
    */
    //return (true);
//}

/*
Socket* Webserv::find_listen_socket(int socket_fd)
{
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        if (this->_sockets[i]->get_socket_fd() == socket_fd) {
            return (this->_sockets[i]);
        }
    }
    return (NULL);
}
*/

//void Webserv::connected_communication(int fd, struct epoll_event* event, Socket* socket)
//{
	//(void)fd;
	//(void)event;
	//(void)socket;
	/*

    EpollController* epoll_controller = EpollController::get_instance();

    if (event->events & EPOLLIN) {
        try{
            Request* req = socket->recv(fd);
            DEBUG("EPOLLIN fd:" + Utility::to_string(fd));
            req->read_up_to_header();
            req->print_request();

            if (req->get_state() != LOADED_STATUS_BODY) {
                MYDEBUG("LOADED_STATUS_BODY is not END_LOAD , state=" + Utility::to_string(req->get_state()) + ", fd:" + Utility::to_string(fd));
                return;
            }

            // Body Test
            // Test (will remove)
            req->print_request();
            if (req->get_content_type().is_multipart()) {
                vector<ByteVector> list = req->get_body_splitted();
                for (size_t i = 0; i < list.size(); i++) {
                    cout << "body[" << i << "]:" << list[i] << endl;
                }
            } else {
                std::cout << "body: <--" << req->get_body().get_str() << "-->" << std::endl;
            }

            if (req->is_cgi())
            {
                CGI* cgi = new CGI(req);
                int read_fd = cgi->get_result_fd();
                RequestCGI *req_cgi = cgi->get_result();
                socket->set_request(read_fd, req_cgi);
                epoll_controller->add(read_fd, EPOLLIN);

            } else {

                Response* res = new Response(req);
                socket->set_response(fd, res);
        cout << "connected_communication No.12" << endl;
                // server processing except cgi
                // socket
                //if (req->get_content_length() > static_cast<ssize_t>(req->get_body().size())) {
                    //cout << "connected_communication not change OUT" << endl;
                    //return;
                //}
                //socket->erase_request(fd);
                //
                //
                //event->events = EPOLLOUT;
                //if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, fd, event) != 0) {
                    //cout << strerror(errno) << endl;
                //}
                epoll_controller->modify(fd, EPOLLOUT);
            }

        }catch(std::exception &e){

            socket->erase_request(fd);
            Response* res = new Response(500);
            socket->set_response(fd, res);
            epoll_controller->modify(fd, EPOLLOUT);
        }
    } else if (event->events & EPOLLOUT) {
        DEBUG("EPOLLOUT fd:" + Utility::to_string(fd));
    cout << "connected_communication No.13" << endl;
        bool write_all = socket->send_response(fd);
        // todo
        if (write_all == false)
            return;

        // sock_fds.erase(fd);
        socket->erase_response(fd);
        socket->erase_request(fd);
        //close(fd);
        //socket->erase_fd(fd);
        //this->_fd_sockets.erase(fd);
        epoll_controller->erase(fd);
        //if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, event) != 0) {
            //cout << "connected_communication not change IN" << endl;
        //}
        // close(fd);
    } else if (event->events & (EPOLLHUP)){
        cout << "connected_communication No.14 EPOLLRDHUP" << endl;
        char tmp[2048];
        int result = read(fd, tmp, 1000);
        cout << "result No.1:" << result << endl;
        tmp[result] = '\0';
        cout << "tmp:" << tmp << endl;
        exit(1);
    } else if (event->events & (EPOLLERR)){
        cout << "connected_communication No.15 EPOLLRDHUP" << endl;
    } else if (event->events & (EPOLLRDHUP | EPOLLERR | EPOLLHUP)) {
        cout << "connected_communication No.15" << endl;
        //todo remove fd
        socket->erase_response(fd);
        socket->erase_request(fd);
        epoll_controller->erase(fd);
        close(fd);
    }
*/
//}

//void Webserv::timeout(int time_sec)
//{
    //(void)time_sec;
    /*
    Socket* sock;
    for (size_t i = 0; i < this->_sockets.size(); i++) {
        sock = this->_sockets[i];
        std::vector<int> delete_fd = sock->timeout(time_sec);
        for (size_t i = 0; i < delete_fd.size(); i++) {
            int tmp_fd = delete_fd[i];
            sock->erase_fd(tmp_fd);
            this->_fd_sockets.erase(tmp_fd);
            WARNING("Timeout! close fd and delete relatived object: close fd=" + tmp_fd);
        }
    }
    */
//}

//size_t Webserv::count_fd()
//{
    //timeout
    /*
    size_t cnt = 0;
    map<int, Socket*>::iterator ite = _fd_sockets.begin();
    map<int, Socket*>::iterator end = _fd_sockets.end();
    for(; ite != end; ite++){
        cnt += ite->second->count_fd();
    }
    return (cnt);
    */
    //return (0);
//}

//void Webserv::close_all_fd()
//{
    //map<int, Socket*>::iterator ite = _fd_sockets.begin();
    //map<int, Socket*>::iterator end = _fd_sockets.end();
    /*
    for(; ite != end; ite++){
        ite->second->erase_all_fd();
    }
    */
    //this->_fd_sockets.clear();
//}
//

void Webserv::communication()
{
    DEBUG("Webserv::communication() start");
    while(1)
    {
        if(waiter.is_not_busy()){
            DEBUG("Webserv::wait() ");
            waiter.wait();
        }
        WebservEvent *event = waiter.serve_event();

        bool flag = false;
        switch(event->which())
        {
            case READ_EVENT:
                DEBUG("Webserv::Read Event ");
                reader.read(event);
                parser.parse_req(event);
                break;
            case APPLICATION_EVENT:
                DEBUG("Webserv::Application Event ");
                //WebservApplication app(event);
                executer.execute(event);
                parser.parse_res(event);
                break;
            case WRITE_EVENT:
                DEBUG("Webserv::Write Event ");
                //WebservSender sender(event);
                sender.send(event);
                break;
            case CLEAN_EVENT:
                DEBUG("Webserv::Clean Event ");
                flag = true;
                cleaner.clean(event);
                //post_processing(event);
                /*
                */
                //close(event);
                break;
            case TIMEOUT_EVENT:
                DEBUG("Webserv::Timeout Event");
                delete(event);
                //WebservSender sender(event);
                //sender.send(event);
                break;
            case NOTHING_EVENT:
                DEBUG("Webserv::Nothing Event");
                delete(event);
                //WebservSender sender(event);
                //sender.send(event);
                break;
        }
        if(flag){
            break;
        }
    }

    // wait()  EpollController class(add,modify,remove) -> (epoll-manager -> socket_repository ), map<event, fd>
    //              map<event, request>
    //              map<event, response>
    //
    // if(READ)
    //      parse Request  RequestParser<fd, event Request>
    //      execute server process ServerController
    //      create response ResponseFactory -> 
    // else if(WRITE)
    //      send response 

    /*
    DEBUG("Webserv::communication() start");

    size_t size = this->_sockets.size();
    std::vector<t_epoll_event> events(size);
    struct epoll_event server_event;
    //EpollController* epoll_controller = EpollController::get_instance();

    Utility::memset(&server_event, 0, sizeof(struct epoll_event));
    */
    //if (this->init_epoll() == false) {
        //ERROR("failure to init_epoll()");
        //return;
    //}
    /*
    while (1) {
        int time_msec = -1;
        if (this->_fd_sockets.size() > 0) {
            time_msec = 5;
        }
        if (events.size() != epoll_controller->get_fd_num()){
            events.resize(epoll_controller->get_fd_num());
        }
        int nfds = epoll_controller->wait(&(events[0]));
        cout << "nfds:" << nfds << ", count fd:" << this->count_fd() << endl;

        if (this->count_fd() > MAX_FD){
            WARNING("reach MAX_FD. force to close all fd");
            this->close_all_fd();
        }
        cout << "No.1" << endl;
        if (nfds == 0) {
            this->timeout(time_msec);
            continue;
        } else if (nfds < 0) {
            ERROR("Epoll Wait Error nfds < 0:"  );
            cout << "Epoll Wait Error:" << strerror(errno) << endl;
            return;
        }
        cout << "No.2" << endl;
        for (int i = 0; i < nfds; i++) {
            int fd = events[i].data.fd;
            cout << "No.3 fd:" << fd << endl;
            std::map<int, Socket*>::iterator tmp_fd = this->_fd_sockets.find(events[i].data.fd);
            //std::map<int, CGI*>::iterator cgi_fd = this->_fd_cgi.find(events[i].data.fd);

            if (tmp_fd != this->_fd_sockets.end()) {
                DEBUG("find Socket fd:" + Utility::to_string(fd) + ".and execute connected_communication");
                cout << "No.4" << endl;
                Socket* socket = tmp_fd->second;
                connected_communication(tmp_fd->first, &(events[i]), socket);
                continue;
            }
            if (cgi_fd != this->_fd_cgi.end()) {
                CGI* cgi = cgi_fd->second;
                Response *res = new ResponseCGI(cgi);
                std::map<int, Socket*>::iterator tmp_fd = this->_fd_sockets.find(cgi->);

                socket->set_response(fd, res);
                //todo cgi process

                continue;
            }
            Socket* socket = find_listen_socket(fd);
            if (socket) {
                int new_fd = socket->accept_request();
                this->_fd_sockets.insert(std::make_pair(new_fd, socket));
                epoll_controller->add(new_fd, EPOLLIN);
                continue;
            }
        cout << "No.6" << endl;
        }
    }
    */
}

/*
// ここではget_entries_in_directoryの例外を処理していないので、
// 使う際はget_entries_in_directoryの例外を処理することを念頭に実装してください
string Webserv::get_auto_index_page(string const& path) const
{
    string result =
        "<html>\n<head>\n<title>Index of " + path + "</title>\n</head>\n<body>\n<h1>Index of " + path + "</h1>\n";
    vector<string> entries = FileUtility::get_entries_in_directory(path);
    for (size_t i = 0; i < entries.size(); i++)
        result += "<a href=\"" + entries[i] + "\">" + entries[i] + "</a><br>\n";
    result += "</body>\n</html>\n";
    return (result);
}
*/

void Webserv::reset()
{

}
