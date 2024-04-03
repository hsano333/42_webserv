
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
#include "request.hpp"
#include "response.hpp"
#include "tcp_socket.hpp"
#include "utility.hpp"
#include "log.hpp"
#include "global.hpp"
#include "epoll_controller.hpp"
#include "http_exception.hpp"
#include "connection_exception.hpp"
#include "event_controller.hpp"

using std::cout;
using std::endl;
using std::string;


Webserv::Webserv(
        Config *cfg,
        WebservEventFactory *event_factory,
        EventManager        *event_manager,
        EventController     *event_controller,
        WebservWaiter       &waiter,
        WebservCleaner      *cleaner
        ) :
                     cfg(cfg),
                     event_factory(event_factory),
                     event_manager(event_manager),
                     event_controller(event_controller),
                     waiter(waiter),
                     cleaner(cleaner)
{
    ;
}

Webserv::~Webserv()
{
}

Webserv& Webserv::operator=(const Webserv& socket)
{
    if (&socket == this)
        return (*this);
    return (*this);
}

void Webserv::communication()
{
    DEBUG("Webserv::communication() start");
    bool exit_flag = false;
    int wait_time;
    int count = 0;
    while(1)
    {
        count++;
        if(waiter.is_not_busy()){
            wait_time = 1;
        }else{
            wait_time = 0;
        }
        waiter.wait(wait_time);
        waiter.fetch_events();

        size_t cur_size = this->event_manager->event_size();
        for(size_t i=0; i < cur_size; i++)
        {
            WebservEvent *event = this->event_manager->pop_first();
            if(event == NULL){
                continue;
            }

            WebservEvent *next_event = NULL;

            try{

                DEBUG("start handle()");
                handle(event);
                DEBUG("end handle()");
                if(event->entity()->completed()){
                    DEBUG("make_next_event");
                    next_event = make_next_event(event, this->event_factory);
                }else{
                    DEBUG("next is current event");
                    next_event = event;
                }
                    DEBUG("set_next_epoll_event No.1");
                event_controller->set_next_epoll_event(event, next_event);
                    DEBUG("set_next_epoll_event No.2");
            }catch(ConnectionException &e){
                ERROR(e.what());
                next_event = this->event_factory->make_clean_event(event, true);
                this->event_manager->push(next_event);
                DEBUG("ConnectionException delete event:" + Utility::to_string(event));
            }catch(HttpException &e){
                ERROR("HttpException:" + Utility::to_string(e.what()));
                next_event = this->event_factory->make_event_from_http_error(event, e.what());
                this->event_manager->push(next_event);
                try{
                    this->event_controller->change_write_event(next_event);
                }catch(std::runtime_error &e){
                    ERROR("IO Error in HttpException: end event");
                    next_event = this->event_factory->make_clean_event(event, true);
                    this->event_manager->push(next_event);
                }
            }catch(std::runtime_error &e){
                WARNING("Wevserv RuntimeError:");
                WARNING(e.what());
                next_event = this->event_factory->make_clean_event(event, false);
                this->event_manager->push(next_event);
            }catch(std::invalid_argument &e){
                WARNING("Wevserv InvalidArgument:");
                WARNING(e.what());
                next_event = this->event_factory->make_clean_event(event, false);
                event_manager->push(next_event);
            }catch(std::exception &e){
                WARNING("Wevserv Exception:");
                WARNING(e.what());
                next_event = this->event_factory->make_clean_event(event, false);
                event_manager->push(next_event);
            }

            if(next_event != event){
                this->cleaner->delete_event(event);
                count++;
            }
        }

        if(exit_flag || count > 100){
        }
    }
}

void Webserv::reset()
{
    ;
}
