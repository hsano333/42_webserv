#include "webserv_sender.hpp"
#include "webserv_write_event.hpp"

WebservSender::WebservSender(
            IOMultiplexing *io_multi_controller,
            FDManager *fd_manager,
            WebservEventFactory *event_factory,
            EventManager *event_manager
        ):
        io_multi_controller(io_multi_controller),
        fd_manager(fd_manager),
        event_factory(event_factory),
        event_manager(event_manager)
{
    ;
}

WebservSender::~WebservSender()
{
    ;
}

void WebservSender::send(WebservEvent *event)
{
    WebservWriteEvent *write_event = static_cast<WebservWriteEvent*>(event);
    Response *res = write_event->res();
    res->print_info();
    //std::string str = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length:5\r\n\r\nabcde\r\n";
    std::string str = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nContent-Length:5\r\nabcde\r\n";
    const char *str2 = str.c_str();
    write_event->write(str2, str.size());


    //(void)event;
    WebservEvent *next_event = this->event_factory->make_clean_event(event);
    event_manager->push(next_event);
    delete (event);
}
