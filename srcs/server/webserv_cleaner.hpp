#ifndef WEBSERV_CLEANER_HPP
#define WEBSERV_CLEANER_HPP
#include "webserv_event.hpp"
#include "webserv_entity.hpp"
#include "io_multiplexing.hpp"
#include "event_manager.hpp"
#include "event_controller.hpp"
#include "file_manager.hpp"
#include "fd_manager.hpp"
#include "file_manager.hpp"

class EventManager;
class WebservEntity;
class WebservEvent;
class EventController;
class WebservCleaner
{
    public:
        WebservCleaner(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    EventController *event_controller,
                    FDManager *fd_manager,
                    FileManager *file_manager
                );
        ~WebservCleaner();
        bool delete_event(WebservEvent *event);
        void clean(WebservEntity *entity, bool force_close);
        void close_fd(FileDiscriptor const &fd);
    private:
        IOMultiplexing *io_multi_controller;
        EventManager *event_manager;
        EventController *event_controller;
        FDManager *fd_manager;
        FileManager *file_manager;

};

#endif
