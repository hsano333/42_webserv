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
    DEBUG("WebservSender::send() fd:" + event->get_fd().to_string());
    WebservWriteEvent *write_event = static_cast<WebservWriteEvent*>(event);
    std::string str = "HTTP/1.1 200 OK\r\nContent-Length:5\r\nabcde\r\n";
    const char *str2 = str.c_str();
    DEBUG("WebservSender::send() No.1");
    cout << "str2:" << str2 << endl;
    write_event->write(str2, str.size());
    DEBUG("WebservSender::send() No.2");
    //(void)event;
    WebservEvent *next_event = this->event_factory->make_clean_event(event);
    event_manager->push(next_event);
    DEBUG("WebservSender::send() fdNo.2:" + next_event->get_fd().to_string());
    delete (event);
    DEBUG("WebservSender::send() No.2");
}
