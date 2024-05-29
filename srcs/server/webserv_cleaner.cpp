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
    DEBUG("WebservCleaner::clean() force_close:" + Utility::to_string(force_close));

    FileDiscriptor fd = entity->fd();
    DEBUG("WebservCleaner::clean fd:" + Utility::to_string(fd));
    DEBUG("WebservCleaner::clean fd:" + fd.to_string());

    // delete all registered files;
    file_manager->erase(fd);

    // delete all registered events;
    event_manager->erase_events_will_deleted(fd);

    if(entity){
        DEBUG("delete entity in clean event address::" + Utility::to_string(entity));
        DEBUG("end delete entity in clean event");
    }

    // except keep-alive
    if(force_close){
        DEBUG("force close ::clean fd:" + Utility::to_string(fd));
        DEBUG("force close:" + fd.to_string());
        fd.close();
        delete entity;
    }

}

bool WebservCleaner::delete_event(WebservEvent *event, WebservEvent *next_event)
{
    DEBUG("delete_event register event :" + Utility::to_string(event));
    //event_manager->add_events_will_deleted(event->entity()->fd(), event);
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


