#include "webserv_sender.hpp"
#include "webserv_write_event.hpp"
#include "global.hpp"
#include "connection_exception.hpp"
#include "http_exception.hpp"

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

    Response *res = write_event->res();
    char data[MAX_BUF];
    char* p_data = &(data[0]);

    try{
        res->open_file();
    }catch(std::runtime_error &e){
        ERROR("WebservSender::send():" + string(e.what()));
        throw HttpException("403");
    }

    FileDiscriptor fd = write_event->fd();
    ssize_t size = res->get_data(&p_data);
    p_data[size] = '\0';
    while(size > 0)
    {
        //cout << p_data << endl;
        int result = writer->write(fd, p_data, size);
        if (result < 0){
            throw ConnectionException("Write Error");
        }else if(result == 0){
            break;
        }
        p_data = &(data[0]);
        size = res->get_data(&p_data);
        p_data[size] = '\0';
        cout << "data=[" << p_data << "]" << endl;
    }

    res->close_file();

    WebservEvent *next_event = this->event_factory->make_clean_event(event, false);
    event_manager->push(next_event);
    delete (event);
    DEBUG("WebservSender::send() end");
}
