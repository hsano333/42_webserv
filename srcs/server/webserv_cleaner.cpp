#include "webserv_cleaner.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "header_word.hpp"

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

void WebservCleaner::clean_timeout_events()
{
    DEBUG("WebservCleaner::clean_timeout_events()");
    std::vector<WebservEvent *> timeout_events;

    this->event_manager->retrieve_timeout_events(timeout_events);
    for(size_t i=0;i<timeout_events.size();i++){
        this->clean(timeout_events[i]->entity(), true);
    }
}

void WebservCleaner::clean(WebservEntity *entity, bool force_close)
{
    DEBUG("WebservCleaner::clean()");

    //entityを削除すると、後工程で問題が発生するので、ここでは消さない
    bool is_close = force_close || entity->force_close();
    if (entity->request()){
        std::string const &conect = entity->request()->header().find(CONNECTION);
        if (conect == "close"){
            is_close = true;
        }
    }

    /*
    //動的確保したファイルの削除
    file_manager->erase(entity->fd());
    if (is_close)
    {
        MYINFO("close fd:" + entity->fd().to_string());
        //ヘッダーでcloseするように指定されているので、closeする
        //this->fd_manager->close_fd(entity->fd());
    }else{
        MYINFO("not close fd:" + entity->fd().to_string());
        // HTTP1.1はデフォルトでコネクションを切断しない
    }
    */
    entity->set_force_close(is_close);
    entity->set_completed(true);
}

void WebservCleaner::delete_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservCleaner::delete_event");
    //if(event){
        //delete event;
    //}
}

/*
bool WebservCleaner::clean(WebservEvent *event)
{
    bool clean_event = false;
    DEBUG("WebservCleaner::clean");
    //動的確保したファイルの削除
    if(event->which() == CLEAN_EVENT)
    {
        clean_event = true;
        WebservEntity *entity = event->entity();
        file_manager->erase(entity->fd());
        MYINFO("WebservCleaner::clean event");
        if(entity->force_close()){
            MYINFO("WebservCleaner:: force close");
            this->fd_manager->close_fd(entity->fd());
        }
    }
    delete event;
    return (clean_event);
}
*/

void WebservCleaner::close_fd(FileDiscriptor const &fd)
{
    fd_manager->close_fd(fd);
}


#include "webserv_timeout_event.hpp"
#include "webserv_entity.hpp"

