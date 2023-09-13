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

