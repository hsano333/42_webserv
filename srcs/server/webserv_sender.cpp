#include "webserv_sender.hpp"
#include "webserv_write_event.hpp"

WebservSender::WebservSender(
            IOMultiplexing *io_multi_controller,
            FDManager *fd_manager
        ):
        io_multi_controller(io_multi_controller),
        fd_manager(fd_manager)
{
    ;
}

WebservSender::~WebservSender()
{
    ;
}

void WebservSender::send(WebservEvent *event)
{
    DEBUG("WebservSender::send()");
    WebservWriteEvent *write_event = static_cast<WebservWriteEvent*>(event);
    std::string str = "HTTP/1.1 200 OK\r\nContent-Length:0\r\n\r\n";
    const char *str2 = str.c_str();
    write_event->write(str2, str.size());
    this->io_multi_controller->erase(event->get_fd());
    this->fd_manager->close_fd(event->get_fd());
    delete (event);
    (void)event;
    ;
}
