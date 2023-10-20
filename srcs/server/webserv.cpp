
#include "webserv.hpp"

#include <errno.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "cgi.hpp"
//#include "file_utility.hpp"
#include "request.hpp"
#include "response.hpp"
//#include "response_cgi.hpp"
#include "tcp_socket.hpp"
#include "utility.hpp"
#include "log.hpp"
#include "global.hpp"
#include "epoll_controller.hpp"
#include "http_exception.hpp"

#define NEVENTS 16
using std::cout;
using std::endl;
using std::string;


Webserv::Webserv(
        Config *cfg,
        SocketManager *socket_manager,
        WebservEventFactory *event_factory,
        EventManager        *event_manager,
        //EpollController epoll_controller,
        WebservWaiter &waiter,
        WebservReceiver &receiver,
        WebservParser &parser,
        WebservExecuter &executer,
        WebservSender &sender,
        WebservCleaner &cleaner
        ) :
                     //_epfd(0),
                     cfg(cfg),
                     socket_manager(socket_manager),
                     event_factory(event_factory),
                     event_manager(event_manager),
                     //epoll_controller(epoll_controller),
                     waiter(waiter),
                     receiver(receiver),
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

Webserv& Webserv::operator=(const Webserv& socket)
{
    if (&socket == this)
        return (*this);
    return (*this);
}


void Webserv::communication()
{
    //for test
    bool exit_flag = false;
    DEBUG("Webserv::communication() start");

    /*
    struct sockaddr_in sin;
    socklen_t len = sizeof(sin);

    int sockfd = 3;

    std::vector<int> vec;
    vec.push_back(3);
    vec.push_back(5);
    vec.push_back(6);
    vec.push_back(7);
    std::vector<int>::iterator ite = vec.begin();
    std::vector<int>::iterator end = vec.end();

    while(ite != end){
        sockfd = *ite;
        int rval = getsockname(sockfd, (struct sockaddr*)&sin, &len);
        cout << "rval:" << rval << endl;
        cout << sin.sin_family << endl;
        cout << "port:" << ntohs(sin.sin_port) << endl;
        cout << "addr:" << ntohs(sin.sin_addr.s_addr) << endl;
        //cout << sin.sa_data << endl;
        printf("tmpsockaddr.sa_family=%d\n", sin.sin_family);
        //printf("tmpsockaddr.sin_port=%s\n", ntohs(sin.sin_port));
        printf("tmpsockaddr.sa_date=%d\n", sin.sin_addr.s_addr);
        printf("tmpsockaddr.sin_zero=%s\n", sin.sin_zero);
        printf("len=%d\n", len);
        ite++;
    }
    */

    while(1)
    {
        if(waiter.is_not_busy()){
            DEBUG("Webserv::wait()");
            waiter.wait();
        }
        WebservEvent *event = waiter.serve_event();
        try{
            switch(event->which())
            {
                case READ_EVENT:
                    DEBUG("Webserv::Read Event ");
                    receiver.receiver(event);
                    parser.parse_req(event);
                    break;
                case APPLICATION_EVENT:
                    DEBUG("Webserv::Application Event ");
                    executer.execute(event);
                    //parser.parse_res(event);
                    break;
                case WRITE_EVENT:
                    DEBUG("Webserv::Write Event ");
                    sender.send(event);
                    break;
                case CLEAN_EVENT:
                    DEBUG("Webserv::Clean Event ");
                    cleaner.clean(event);
                    break;
                case TIMEOUT_EVENT:
                    DEBUG("Webserv::Timeout Event");
                    cleaner.clean_timeout_events(event);
                    delete(event);
                    break;
                case NOTHING_EVENT:
                    DEBUG("Webserv::Nothing Event");
                    delete(event);
                    break;
            }
            if(exit_flag){
                break;
            }
        }catch(HttpException &e){
            std::string code_str = e.what();
            StatusCode code;
            try{
                code = StatusCode::from_string(code_str);
            }catch (std::runtime_error &e){
                //code_str = "500";
                code = StatusCode::from_string("500");
            }
            WebservEvent *next_event = this->event_factory->make_error_event(event, code);
            this->event_manager->push(next_event);
        }catch(std::runtime_error &e){
            WARNING("Wevserv RuntimeError:");
            WARNING(e.what());
            WebservEvent *next_event = this->event_factory->make_clean_event(event);
            this->event_manager->push(next_event);
        }catch(std::invalid_argument &e){
            WARNING("Wevserv InvalidArgument:");
            WARNING(e.what());
            WebservEvent *next_event = this->event_factory->make_clean_event(event);
            event_manager->push(next_event);
        }catch(std::exception &e){
            WARNING("Wevserv Exception:");
            WARNING(e.what());
            WebservEvent *next_event = this->event_factory->make_clean_event(event);
            event_manager->push(next_event);
        }
    }
}

void Webserv::reset()
{

}
