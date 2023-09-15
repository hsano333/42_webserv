#include "webserv_cleaner.hpp"

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
    DEBUG("WebservCleaner::clean:" + event->get_fd().to_string());
    delete event->req();
    delete event->res();
    this->io_multi_controller->erase(event->get_fd());
    this->fd_manager->close_fd(event->get_fd());
    this->event_manager->erase_event_waiting_writing(event->get_fd());
    delete event;
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

