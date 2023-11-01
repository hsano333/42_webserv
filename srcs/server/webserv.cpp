
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
#include "connection_exception.hpp"
#include "event_controller.hpp"

#define NEVENTS 16
using std::cout;
using std::endl;
using std::string;


Webserv::Webserv(
        Config *cfg,
        //SocketManager *socket_manager,
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
        ) :
                     //_epfd(0),
                     cfg(cfg),
                     //socket_manager(socket_manager),
                     event_factory(event_factory),
                     event_manager(event_manager),
                     event_controller(event_controller),
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


    int cnt = 0;
    while(1)
    {
        if(waiter.is_not_busy()){
            DEBUG("Webserv::wait()");
            cout << "gai_strerror:" << gai_strerror(errno) << endl;

            waiter.wait();
        }
        WebservEvent *event = waiter.serve_event();
        try{
            switch(event->which())
            {
                case READ_EVENT:
                    DEBUG("Webserv::Read Event ");
                    receiver.recv(event);
                    parser.parse_req(event);
                    event_controller->next_event(event);
                    break;
                case APPLICATION_EVENT:
                    DEBUG("Webserv::Application Event ");
                    executer.execute(event);
                    event_controller->next_event(event);
                    //parser.parse_res(event);
                    break;
                case WRITE_EVENT:
                    DEBUG("Webserv::Write Event ");
                    sender.send(event);
                    event_controller->next_event(event);
                    break;
                case CLEAN_EVENT:
                    DEBUG("Webserv::Clean Event ");
                    cleaner.clean(event, false);
                    event_controller->next_event(event);

                    if (cnt >= 10){
                        //delete event;
                        cout << "end break" << endl;
                        cout << "end break" << endl;
                        cout << "end break" << endl;
                        cout << "end break" << endl;
                        cout << "end break" << endl;
                        //delete event;
                        return ;
                        break;
                    }
                    cnt++;
                    break;
                case TIMEOUT_EVENT:
                    DEBUG("Webserv::Timeout Event");
                    cleaner.clean_timeout_events(event);
                    event_controller->next_event(event);
                    //delete(event);
                    break;
                    /*
                case KEEPA_ALIVE_EVENT:
                    DEBUG("Webserv::Keep Alive Event");
                    event_controller->restart_communication(event);
                    DEBUG("Webserv::Keep Alive Event No.1");
                    event_controller->next_event(event);
                    DEBUG("Webserv::Keep Alive Event No.2");
                    // nothing to do
                    break;
                    */
                case NOTHING_EVENT:
                    DEBUG("Webserv::Nothing Event");
                    event_controller->next_event(event);
                    //delete(event);
                    break;
                default:
                    break;
            }
            if(exit_flag){
                break;
            }
        }catch(ConnectionException &e){
            ERROR(e.what());
            WebservEvent *next_event = this->event_factory->make_clean_event(event, true);
            this->event_manager->push(next_event);
            delete event;
            DEBUG("ConnectionException delete event:" + Utility::to_string(event));
        }catch(HttpException &e){
            ERROR("HttpException");
            WebservEvent *next_event = this->event_factory->make_error_event(event, e.what());
            this->event_manager->push(next_event);
            this->event_controller->change_write_event(next_event);
            delete event;
        }catch(std::runtime_error &e){
            WARNING("Wevserv RuntimeError:");
            WARNING(e.what());
            WebservEvent *next_event = this->event_factory->make_clean_event(event, false);
            this->event_manager->push(next_event);
            delete event;
        }catch(std::invalid_argument &e){
            WARNING("Wevserv InvalidArgument:");
            WARNING(e.what());
            WebservEvent *next_event = this->event_factory->make_clean_event(event, false);
            event_manager->push(next_event);
            delete event;
        }catch(std::exception &e){
            WARNING("Wevserv Exception:");
            WARNING(e.what());
            WebservEvent *next_event = this->event_factory->make_clean_event(event, false);
            event_manager->push(next_event);
            delete event;
        }
    }
}

void Webserv::reset()
{

}
