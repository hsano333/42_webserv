#include "webserv_cleaner.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "header_word.hpp"
#include "webserv_timeout_event.hpp"
#include "webserv_entity.hpp"
#include "io_multiplexing.hpp"


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
        if(entity->io().get_write_fd().to_int() > 0){
            this->event_controller->erase_epoll_event(entity->io().get_write_fd());
            //entity->io().set_write_fd(FileDiscriptor());
        }
        if(entity->io().get_read_fd().to_int() > 0){
            this->event_controller->erase_epoll_event(entity->io().get_read_fd());
            //entity->io().set_read_fd(FileDiscriptor());
        }

        if(entity->event_error() == Timeout){
            entity->app_result()->pid().kill();
        }

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
    std::vector<FileDiscriptor> fd_vec;
    event_manager->erase_events_will_deleted_except_keepout(fd, &fd_vec);
    for(size_t i=0;i<fd_vec.size();i++){
        this->io_multi_controller->erase(fd_vec[i]);
        fd_vec[i].close();
    }

    //if(entity){
        //DEBUG("delete entity in clean event address::" + Utility::to_string(entity));
        //DEBUG("end delete entity in clean event");
    //}

    // except keep-alive
    entity->set_request(NULL);
    entity->set_response(NULL);
    entity->set_result(NULL);
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
        fd.close();
        if(entity->io().get_write_fd().to_int() > 0){
            DEBUG("close cgi pipe:" + entity->io().get_write_fd().to_string());
            entity->io().get_write_fd().close();
        }
        if(entity->io().get_read_fd().to_int() > 0){
            DEBUG("close cgi pipe:" + entity->io().get_write_fd().to_string());
            entity->io().get_read_fd().close();
        }
        delete entity;
    }

}

bool WebservCleaner::delete_event(WebservEvent *event)
{
    DEBUG("delete_event register event :" + Utility::to_string(event));
    this->clean(event->entity(), event->entity()->force_close());
    //event_manager->add_events_will_deleted(event->entity()->fd(), event);
    //if(event->which() == CLEAN_EVENT){
        //DEBUG("WebservCleaner::delete_event");
        //this->clean(event->entity(), event->entity()->force_close());
        //return (true);
    //}else if(event != next_event){
        //DEBUG("delete");
    //}
    return (true);
}

void WebservCleaner::close_fd(FileDiscriptor const &fd)
{
    DEBUG("WebservCleaner::close fd:" + fd.to_string());
    fd_manager->close_fd(fd);
}


