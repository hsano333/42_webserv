#include "webserv_cleaner.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "header_word.hpp"
#include "webserv_timeout_event.hpp"
#include "webserv_entity.hpp"


using std::cout;
using std::endl;

WebservCleaner::WebservCleaner(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    FDManager *fd_manager,
                    FileManager *file_manager
                    ):
                    io_multi_controller(io_multi_controller),
                    event_manager(event_manager),
                    fd_manager(fd_manager),
                    file_manager(file_manager)
{
    ;
}
WebservCleaner::~WebservCleaner()
{
    ;
}

void WebservCleaner::clean(WebservEntity *entity, bool force_close)
{
    DEBUG("WebservCleaner::clean()");

    FileDiscriptor const &fd = entity->fd();

    // delete all registered files;
    file_manager->erase(fd);

    // delete all registered events;
    event_manager->erase_events_will_deleted(fd);

    if(entity){
        delete entity;
    }

    // except keep-alive
    if(!force_close){
        fd.close();
    }

}

bool WebservCleaner::delete_event(WebservEvent *event, WebservEvent *next_event)
{
    event_manager->add_events_will_deleted(event->entity()->fd(), event);
    if(event->which() == CLEAN_EVENT){
        DEBUG("WebservCleaner::delete_event");
        this->clean(event->entity(), event->entity()->force_close());
        return (true);
    }else if(event != next_event){
        DEBUG("delete");
    }
    return (false);
}

void WebservCleaner::close_fd(FileDiscriptor const &fd)
{
    DEBUG("WebservCleaner::close fd:" + fd.to_string());
    fd_manager->close_fd(fd);
}


