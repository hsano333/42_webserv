#ifndef WEBSERV_CLEANER_HPP
#define WEBSERV_CLEANER_HPP
#include "webserv_event.hpp"
#include "webserv_entity.hpp"
#include "io_multiplexing.hpp"
#include "event_manager.hpp"
#include "fd_manager.hpp"
#include "file_manager.hpp"
//#include "webserv_timeout_event.hpp"
//#include "webserv_clean_event.hpp"

class EventManager;
class WebservEntity;
class WebservEvent;
class WebservCleaner
{
    public:
        WebservCleaner(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    FDManager *fd_manager,
                    FileManager *file_manager
                );
        ~WebservCleaner();
        void delete_event(WebservEvent *event);
        void clean(WebservEntity *entity, bool force_close);
        //bool clean(WebservEvent *event);
        void close_fd(FileDiscriptor const &fd);
        void clean_timeout_events();
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        FDManager *fd_manager;
        FileManager *file_manager;

};

//template<typename EventPointer>
//class WebservCleanEvent;
//class WebservEntity;
//void clean(WebservCleanEvent *event, WebservEntity *entity);


//class WebservTimeoutEvent;
//void force_clean(WebservTimeoutEvent *event);
//void clean_timeout_events(WebservTimeoutEvent *event, WebservEntity *entity);
//void clean(WebservEvent *event);


#endif
