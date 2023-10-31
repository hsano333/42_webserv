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
    DEBUG("No.1 WebservSender::send()");

    Response *res = write_event->res();
    DEBUG("No.2 WebservSender::send()");
    if(res == NULL){
        ERROR("WebservSender::send(): Response is NULL");
        throw HttpException("500");
    }
    DEBUG("No.3 WebservSender::send()");

    //printf("res=%p\n", res);
    char data[MAX_BUF];
    char* p_data = &(data[0]);
    DEBUG("No.4 WebservSender::send()");

    try{
    DEBUG("No.41 WebservSender::send()");
        res->open_file();
    DEBUG("No.42 WebservSender::send()");
    }catch(std::runtime_error &e){
        //ERROR("WebservSender::send():" + string(e.what()));
        //throw HttpException("403");
    }
    DEBUG("No.5 WebservSender::send()");

    FileDiscriptor fd = write_event->fd();
    ssize_t size = res->get_data(&p_data);
    p_data[size] = '\0';
    DEBUG("No.6 WebservSender::send()");
    while(size > 0)
    {
    DEBUG("No.7 WebservSender::send()");
        int result = writer->write(fd, p_data, size, NULL);
    DEBUG("No.8 WebservSender::send()");
        if (result < 0){
            ERROR("WebservSender::send() result:" + Utility::to_string(result));
            throw ConnectionException("Write Connection Error");
            //todo
            //event->set_next_flag(true);
            //this->event_manager->add_event_waiting_writing(fd, event);
        }else if(result == 0){
            event->set_completed(false);
            break;
        }
    DEBUG("No.9 WebservSender::send()");
        p_data = &(data[0]);
        size = res->get_data(&p_data);
        p_data[size] = '\0';
        cout << "data=[" << p_data << "]" << endl;
    }
    DEBUG("No.10 WebservSender::send()");

    //todo
    //event->set_next_flag(true);
    res->close_file();
    event->set_completed(true);
    DEBUG("WebservSender::send() end");
}
