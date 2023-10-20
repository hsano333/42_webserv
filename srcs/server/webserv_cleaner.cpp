#include "webserv_cleaner.hpp"
#include "webserv_clean_event.hpp"

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
void WebservCleaner::clean(WebservEvent *event)
{
    WebservCleanEvent *app_event = static_cast<WebservCleanEvent*>(event);
    DEBUG("WebservCleaner::clean:" + event->fd().to_string());
    cout << "clean No.1 " << endl;
    printf("event->req()=%p\n", app_event->req());
    printf("event->res()=%p\n", app_event->res());

    Request *req = app_event->req();
    req->print_info();
    //delete req;
    //exit(1);

    delete app_event->req();
    cout << "clean No.2" << endl;
    delete app_event->res();
    cout << "clean No.3" << endl;
    this->io_multi_controller->erase(app_event->fd());
    cout << "clean No.4" << endl;
    this->fd_manager->close_fd(app_event->fd());
    cout << "clean No.5" << endl;
    this->event_manager->erase_event_waiting_writing(app_event->fd());
    cout << "clean No.6" << endl;
    delete app_event;
}

void WebservCleaner::clean_timeout_events(WebservEvent *event)
{
    (void)event;
    std::vector<WebservEvent *> timeout_events;

    this->event_manager->retrieve_timeout_events(timeout_events);
    for(size_t i=0;i<timeout_events.size();i++){
        this->clean(timeout_events[i]);
    }
}

