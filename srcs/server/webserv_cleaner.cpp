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

    if(entity->app_result()){
        if(entity->io().get_write_fd().to_int() > 0){
            this->event_controller->erase_epoll_event(entity->io().get_write_fd());
        }
        if(entity->io().get_read_fd().to_int() > 0){
            this->event_controller->erase_epoll_event(entity->io().get_read_fd());
        }

        if(entity->event_error() == Timeout){
            entity->app_result()->pid().kill();
        }
    }

    // delete all registered files;
    file_manager->erase(fd);
    std::vector<FileDiscriptor> fd_vec;
    WebservEntity *not_delete_entity = event_manager->erase_events_will_deleted_except_keepout(fd, &fd_vec);
    for(size_t i=0;i<fd_vec.size();i++){
        this->io_multi_controller->erase(fd_vec[i]);
        fd_vec[i].close();
    }


    entity->set_request(NULL);
    entity->set_response(NULL);
    entity->set_result(NULL);
    if(force_close){
        DEBUG("force close ::clean fd:" + Utility::to_string(fd));
        DEBUG("force close:" + fd.to_string());
        try{
            this->io_multi_controller->erase(fd);
        }catch(std::runtime_error &e){
            WARNING("This error is ignored:" + Utility::to_string(e.what()));
        }

        this->fd_manager->close_fd(fd);
        WebservEntity *deleted_entity = event_manager->erase_events_will_deleted_event(fd);

        // メモリリーク対策
        // ここは本来keep alive eventのメモリを解放する場所だが、前段でそれ以外のeventを削除するが、
        // entityだけは削除しない。entityはここで削除するから。
        // しかし、Connection切断時、ここに入るのはkeep alive以外のeventであるが、
        // そのイベントは上段で削除されるため、ここでは削除されないがentityだけが残ってしまう
        if(not_delete_entity && not_delete_entity!= deleted_entity){
            delete not_delete_entity;
        }
        fd.close();
    }

}

bool WebservCleaner::delete_event(WebservEvent *event)
{
    DEBUG("delete_event register event :" + Utility::to_string(event));
    this->clean(event->entity(), event->entity()->force_close());
    return (true);
}

void WebservCleaner::close_fd(FileDiscriptor const &fd)
{
    DEBUG("WebservCleaner::close fd:" + fd.to_string());
    fd_manager->close_fd(fd);
}

