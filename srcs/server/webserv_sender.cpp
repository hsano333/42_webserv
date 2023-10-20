#include "webserv_sender.hpp"
#include "webserv_write_event.hpp"
#include "global.hpp"

WebservSender::WebservSender(
            IOMultiplexing *io_multi_controller,
            FDManager *fd_manager,
            WebservEventFactory *event_factory,
            EventManager *event_manager,
            SocketWriter *writer
        ):
        io_multi_controller(io_multi_controller),
        fd_manager(fd_manager),
        event_factory(event_factory),
        event_manager(event_manager),
        writer(writer)
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
    //Request *req = write_event->req();
    //req->print_info();


    Response *res = write_event->res();
    char data[MAX_BUF];
    //printf("No.1 data=%p\n", data);
    char* p_data = &(data[0]);

    FileDiscriptor fd = write_event->fd();
    ssize_t size = res->get_data(&p_data);
    //delete write_event->res();
    //exit(0);
    while(size > 0){
        writer->write(fd, p_data, size);
        p_data = &(data[0]);
        size = res->get_data(&p_data);
    }

    //delete write_event->res();
    //exit(0);
    WebservEvent *next_event = this->event_factory->make_clean_event(event);
    event_manager->push(next_event);
    delete (event);
}
