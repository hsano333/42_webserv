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
    MYINFO("WebservCleaner::clean: No.1");
    this->io_multi_controller->erase(event->get_fd());
    MYINFO("WebservCleaner::clean: No.2");
    this->fd_manager->close_fd(event->get_fd());
    MYINFO("WebservCleaner::clean: No.3");
    this->event_manager->erase_event_waiting_writing(event->get_fd());
    MYINFO("WebservCleaner::clean: No.4");
    delete event;
    MYINFO("WebservCleaner::clean: No.5");
}

