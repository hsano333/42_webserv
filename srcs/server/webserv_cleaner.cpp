#include "webserv_cleaner.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_read_event.hpp"
#include "webserv_nothing_event.hpp"
#include "webserv_keep_alive_event.hpp"

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

// Socket fd is not cleaned
void WebservCleaner::clean(WebservEvent *event, bool force_close)
{
    WebservCleanEvent *app_event = static_cast<WebservCleanEvent*>(event);
    DEBUG("WebservCleaner::clean:" + event->fd().to_string());

    bool is_close = force_close || app_event->is_force_close();
    if (app_event->req()){
        std::string const &conect = app_event->req()->header().find("Connection");
        if (conect == "close"){
            is_close = true;
        }
    }
    app_event->set_force_close(is_close);

    //this->io_multi_controller->erase(app_event->fd());
    //
    //this->event_manager->erase_event_waiting_epoll(app_event->fd());
    //this->event_manager->erase_event_waiting_epoll(app_event->fd());
    if (is_close)
    {
        MYINFO("close fd:" + event->fd().to_string());
        //ヘッダーでcloseするように指定されているので、closeする
        //this->io_multi_controller->erase(app_event->fd());
        this->fd_manager->close_fd(app_event->fd());
    }else{
        MYINFO("not close fd:" + event->fd().to_string());
        // HTTP1.1はデフォルトでコネクションを切断しない
        //this->io_multi_controller->modify(app_event->fd(), EPOLLIN);
        //WebservEvent *new_event = WebservKeepAliveEvent::from_fd(app_event->fd());
        //this->event_manager->add_event_waiting_epoll(app_event->fd(), new_event);
    }

    event->set_completed(true);
}

void WebservCleaner::clean_timeout_events(WebservEvent *event)
{
    (void)event;
    std::vector<WebservEvent *> timeout_events;

    this->event_manager->retrieve_timeout_events(timeout_events);
    for(size_t i=0;i<timeout_events.size();i++){
        this->clean(timeout_events[i], true);
    }
}

