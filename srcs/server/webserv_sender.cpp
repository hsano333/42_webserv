#include "webserv_sender.hpp"
#include "webserv_write_event.hpp"
#include "global.hpp"
#include "connection_exception.hpp"

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
    p_data[size] = '\0';
    //delete write_event->res();
    //exit(0);
    cout << "sender No.1" << endl;
    DEBUG("WebservSender::send() No.1");
    while(size > 0)
    {
        DEBUG("WebservSender::send() No.1-1");
        cout << p_data << endl;
        DEBUG("WebservSender::send() No.1-2");
        printf("p_data=%p\n", p_data);
        printf("p_data=%s, size=%zu\n", p_data,size);
        int result = writer->write(fd, p_data, size);
        if (result < 0){
            break;
            throw ConnectionException("Write Error");
        }else if(result == 0){
            break;
        }
        p_data = &(data[0]);
        DEBUG("WebservSender::send() No.3:" + Utility::to_string(result));
        size = res->get_data(&p_data);
        DEBUG("WebservSender::send() No.4:" + Utility::to_string(size));
        p_data[size] = '\0';
        DEBUG("WebservSender::send() No.5:" + Utility::to_string(size));
    }
    DEBUG("WebservSender::send() No.2");
    cout << "sender No.2" << endl;
    //writer->write(fd, p_data, size);

    //delete write_event->res();
    //exit(0);
    WebservEvent *next_event = this->event_factory->make_clean_event(event);
    event_manager->push(next_event);
    cout << "sender No.100" << endl;
    delete (event);
    cout << "sender No.101" << endl;
    DEBUG("WebservSender::send() end");
}
