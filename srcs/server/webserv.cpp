
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
        WebservCleaner      &cleaner
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

    bool exit_flag = false;
    DEBUG("Webserv::communication() start");
    //int cnt = 0;
    while(1)
    {
        if(waiter.is_not_busy()){
            waiter.wait(5);
        }else{
            waiter.wait(0);
        }
        waiter.fetch_events();

        size_t cur_size = this->event_manager->event_size();
        for(size_t i=0; i < cur_size; i++)
        {
            WebservEvent *event = this->event_manager->pop_first();
            if(event == NULL){
                break;
            }
            try
            {
                handle(event);
                event_controller->next_event(event);
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
                ERROR("HttpException:" + Utility::to_string(e.what()));
                WebservEvent *next_event = this->event_factory->make_event_from_http_error(event, e.what());
                this->event_manager->push(next_event);
                try{
                    this->event_controller->change_write_event(next_event);
                }catch(std::runtime_error &e){
                    ERROR("IO Error in HttpException: end event");
                    WebservEvent *next_event = this->event_factory->make_clean_event(event, true);
                    this->event_manager->push(next_event);
                }
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
}

void Webserv::reset()
{
;
}
