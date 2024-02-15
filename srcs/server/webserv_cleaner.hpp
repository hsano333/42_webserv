#ifndef WEBSERV_CLEANER_HPP
#define WEBSERV_CLEANER_HPP
#include "webserv_event.hpp"
#include "io_multiplexing.hpp"
#include "event_manager.hpp"
#include "fd_manager.hpp"
#include "webserv_entity.hpp"
//#include "webserv_timeout_event.hpp"
//#include "webserv_clean_event.hpp"

class EventManager;
class WebservEvent;
class WebservCleaner
{
    public:
        WebservCleaner(
                        IOMultiplexing *io_multi_controller,
                        EventManager *event_manager,
                        FDManager *fd_manager

                );
        ~WebservCleaner();
        //void clean(WebservEvent *event, bool force_close);
        //template<typename EventPointer>
        void clean(WebservEvent *event, bool force_close);
        //void clean_timeout_events(WebservEvent *event);
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        FDManager *fd_manager;

};

//template<typename EventPointer>
class WebservCleanEvent;
class WebservEntity;
void clean(WebservCleanEvent *event, WebservEntity *entity);


class WebservTimeoutEvent;
void force_clean(WebservTimeoutEvent *event);
void clean_timeout_events(WebservTimeoutEvent *event, WebservEntity *entity);
//void clean(WebservEvent *event);


#endif
