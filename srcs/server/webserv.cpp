
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
    int wait_time;

#ifdef TEST
    int cnt = 0;
    bool exit_flag = false;
#endif
    while(1)
    {
        wait_time = 0;
        if(waiter.is_not_busy()){
            wait_time = 1;
        }
        waiter.wait(wait_time);
        waiter.fetch_events();

        size_t cur_size = this->event_manager->event_size();
        std::set<WebservEvent*> clean_event;
        for(size_t i=0; i < cur_size; i++)
        {
            WebservEvent *event = this->event_manager->pop_first();
            if(event == NULL){
                continue;
            }

            WebservEvent *next_event = NULL;
            try{
                DEBUG("start handle() event address:" + Utility::to_string(event));
                handle(event);
            }catch(ConnectionException &e){
                ERROR("ConnectionException fd:" + Utility::to_string(event->entity()->fd().to_string()));
                ERROR("ConnectionException:" + Utility::to_string(e.what()));
                event_manager->add_events_will_deleted(event->entity()->fd(), event);

                //event = this->event_factory->make_event_from_http_error(event, e.what());
                event = this->event_factory->make_clean_event(event, true);
                handle(event);
                //this->event_manager->push(next_event);
            }catch(HttpException &e){
                ERROR("HttpException:" + Utility::to_string(e.what()));

                // delete current event, and make new event for error process;
                event_manager->add_events_will_deleted(event->entity()->fd(), event);
                event = this->event_factory->make_event_from_http_error(event, e.what());
                handle(event);
                //delete event;
                //event = error_event;

                /*
                this->event_manager->push(next_event);
                try{
                    this->event_controller->change_write_event(next_event);
                }catch(std::runtime_error &e){
                    ERROR("IO Error in HttpException: end event");
                    next_event = this->event_factory->make_clean_event(event, true);
                    this->event_manager->push(next_event);
                }
                */
            }catch(std::runtime_error &e){
                WARNING("Wevserv RuntimeError:");
                WARNING(e.what());
                next_event = this->event_factory->make_clean_event(event, false);
                //this->event_manager->push(next_event);
            }catch(std::invalid_argument &e){
                WARNING("Wevserv InvalidArgument:");
                WARNING(e.what());
                next_event = this->event_factory->make_clean_event(event, false);
                //event_manager->push(next_event);
            }catch(std::exception &e){
                WARNING("Wevserv Exception:");
                WARNING(e.what());
                next_event = this->event_factory->make_clean_event(event, false);
                //event_manager->push(next_event);
            }


            try{
                if(event->entity()->completed()){
                    next_event = make_next_event(event, this->event_factory);
                    DEBUG("new event:"  + Utility::to_string(next_event));
                }else{
                    DEBUG("next is same event");
                    next_event = event;
                }
                event_controller->set_next_epoll_event(event, next_event);
            }catch(HttpException &e){
                ERROR("HttpException:" + Utility::to_string(e.what()));
                event_manager->add_events_will_deleted(event->entity()->fd(), event);
                event = this->event_factory->make_event_from_http_error(event, e.what());
                handle(event);
                if(event->entity()->completed()){
                    next_event = make_next_event(event, this->event_factory);
                    DEBUG("new event:"  + Utility::to_string(next_event));
                }else{
                    DEBUG("next is same event");
                    next_event = event;
                }
                event_controller->set_next_epoll_event(event, next_event);
            }catch(std::runtime_error &e){
                WARNING("Making Next Event Exception:");
                WARNING(e.what());
                next_event = this->event_factory->make_clean_event(event, true);
                this->event_manager->push(next_event);
                this->event_controller->change_write_event(next_event);
            }

            if(event->which() == CLEAN_EVENT){
                clean_event.insert(event);
            }
        }

        std::set<WebservEvent*>::iterator ite = clean_event.begin();
        std::set<WebservEvent*>::iterator end = clean_event.end();

        while(ite != end){
            WebservEvent *event = *ite;
            if(this->cleaner->delete_event(event)){
#ifdef TEST
                cnt++;
                if(cnt > 7){
                    DEBUG("exit_flag True");
                    exit_flag = true;
                    break;
                }
#endif
            }
            ite++;
        }
#ifdef TEST
        if(exit_flag){
            break;
        }
#endif
    }
    DEBUG("while end");
}

void Webserv::reset()
{
    ;
}
