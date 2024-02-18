#include "webserv_cleaner.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"

using std::cout;
using std::endl;

WebservCleaner::WebservCleaner(
                    IOMultiplexing *io_multi_controller,
                    EventManager *event_manager,
                    FDManager *fd_manager
                    ):
                    io_multi_controller(io_multi_controller),
                    event_manager(event_manager),
                    fd_manager(fd_manager)
{
    ;
}
WebservCleaner::~WebservCleaner()
{
    ;
}

void WebservCleaner::clean_timeout_events()
{
    std::vector<WebservEvent *> timeout_events;

    this->event_manager->retrieve_timeout_events(timeout_events);
    for(size_t i=0;i<timeout_events.size();i++){
        this->clean(timeout_events[i]->entity(), true);
    }
}

void WebservCleaner::clean(WebservEntity *entity, bool force_close)
{
    //WebservCleanEvent *app_event = static_cast<WebservCleanEvent*>(event);
    //DEBUG("WebservCleaner::clean:" + event->fd().to_string());
    //WebservEntity *entity = event->entity();

    bool is_close = force_close || entity->force_close();
    if (entity->request()){
        std::string const &conect = entity->request()->header().find("Connection");
        if (conect == "close"){
            is_close = true;
        }
    }
    //delete entity;
    //entity = NULL;
    //delete entity->request();
    //delete entity->response();
    //app_event->set_null_res_and_req();

    //this->io_multi_controller->erase(app_event->fd());
    //
    //this->event_manager->erase_event_waiting_epoll(app_event->fd());
    //this->event_manager->erase_event_waiting_epoll(app_event->fd());
    if (is_close)
    {
        MYINFO("close fd:" + entity->fd().to_string());
        //ヘッダーでcloseするように指定されているので、closeする
        //this->io_multi_controller->erase(app_event->fd());
        this->fd_manager->close_fd(entity->fd());
    }else{
        MYINFO("not close fd:" + entity->fd().to_string());
        // HTTP1.1はデフォルトでコネクションを切断しない
        //this->io_multi_controller->modify(app_event->fd(), EPOLLIN);
        //WebservEvent *new_event = WebservKeepAliveEvent::from_fd(app_event->fd());
        //this->event_manager->add_event_waiting_epoll(app_event->fd(), new_event);
    }
    //delete entity;
    //event->set_completed(true);
    entity->set_force_close(is_close);
    entity->set_completed(true);
}


void WebservCleaner::close_fd(FileDiscriptor const &fd)
{
    fd_manager->close_fd(fd);
}


#include "webserv_timeout_event.hpp"
#include "webserv_entity.hpp"

