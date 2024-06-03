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
                    EventController *event_controller,
                    FDManager *fd_manager,
                    FileManager *file_manager
                    ):
                    io_multi_controller(io_multi_controller),
                    event_manager(event_manager),
                    event_controller(event_controller),
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

    //close fd of cgi
    if(entity->app_result()){
        this->event_controller->erase_epoll_event(entity->io().get_write_fd());
        this->event_controller->erase_epoll_event(entity->io().get_read_fd());
        /*
        try{
            this->io_multi_controller->erase(entity->io().get_write_fd());
        }catch(std::runtime_error &e){
            WARNING("This error is ignored:" + Utility::to_string(e.what()));
        }
        try{
            this->io_multi_controller->erase(entity->io().get_read_fd());
        }catch(std::runtime_error &e){
            WARNING("This error is ignored:" + Utility::to_string(e.what()));
        }
        */
    }

    // delete all registered files;
    file_manager->erase(fd);


    //if(entity){
        //DEBUG("delete entity in clean event address::" + Utility::to_string(entity));
        //DEBUG("end delete entity in clean event");
    //}

    // except keep-alive
    if(force_close){
        DEBUG("force close ::clean fd:" + Utility::to_string(fd));
        DEBUG("force close:" + fd.to_string());
        //this->io_multi_controller->erase(fd);
        try{
            this->io_multi_controller->erase(fd);
        }catch(std::runtime_error &e){
            WARNING("This error is ignored:" + Utility::to_string(e.what()));
        }

        this->fd_manager->close_socket(fd);
        event_manager->erase_events_will_deleted_event(fd);
        //fd.close();
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


