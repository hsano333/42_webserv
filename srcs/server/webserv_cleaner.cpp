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

    //Request *req = app_event->req();
    //req->print_info();
    //delete req;
    //exit(1);
    //
    

    bool is_close = force_close || app_event->is_force_close();
    if (app_event->req()){
        std::string const &conect = app_event->req()->header().find("Connection");
        if (conect == "close"){
            is_close = true;
        }
    }

    //this->io_multi_controller->erase(app_event->fd());
    //
    this->event_manager->erase_event_waiting_reading(app_event->fd());
    this->event_manager->erase_event_waiting_writing(app_event->fd());
    if (is_close)
    {
        MYINFO("close fd:" + event->fd().to_string());
        cout << "close" << endl;
        //ヘッダーでcloseするように指定されているので、closeする
        this->io_multi_controller->erase(app_event->fd());
        this->fd_manager->close_fd(app_event->fd());
    }else{
        MYINFO("not close fd:" + event->fd().to_string());
        cout << "not close No.1?" << endl;
        // HTTP1.1はデフォルトでコネクションを切断しない
        this->io_multi_controller->modify(app_event->fd(), EPOLLIN);
        WebservEvent *new_event = WebservKeepAliveEvent::from_fd(app_event->fd());
        this->event_manager->add_event_waiting_reading(app_event->fd(), new_event);
    }
    cout << "clean No.5" << endl;

    //this->fd_manager->close_fd(app_event->fd());

    delete app_event->req();
    delete app_event->res();
    delete app_event;
    cout << "clean No.6" << endl;
}

void WebservCleaner::clean_timeout_events(WebservEvent *event)
{
    (void)event;
    std::vector<WebservEvent *> timeout_events;

    this->event_manager->retrieve_timeout_events(timeout_events);
    cout << "timeout_events.size(): " << timeout_events.size() << endl;
    for(size_t i=0;i<timeout_events.size();i++){
        this->clean(timeout_events[i], true);
    }
}

