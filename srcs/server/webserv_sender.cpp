#include "webserv_sender.hpp"
#include "webserv_write_event.hpp"
#include "global.hpp"
#include "connection_exception.hpp"
#include "http_exception.hpp"

WebservSender::WebservSender(
            IOMultiplexing *io_multi_controller,
            //FDManager *fd_manager,
            //WebservEventFactory *event_factory,
            EventManager *event_manager,
            SocketWriter *writer
        ):
        io_multi_controller(io_multi_controller),
        //fd_manager(fd_manager),
        //event_factory(event_factory),
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
    //Response *res = write_event->res();
    File *source = event->src();
    if(source == NULL){
        ERROR("WebservSender::send():  source is NULL");
        throw HttpException("500");
    }
    DEBUG("No.3 WebservSender::send()");

    //printf("res=%p\n", res);
    char data[MAX_BUF];
    char* p_data = &(data[0]);
    DEBUG("No.4 WebservSender::send()");

    try{
        source->open();
    }catch(std::runtime_error &e){
        //ERROR("WebservSender::send():" + string(e.what()));
        //throw HttpException("403");
    }

    FileDiscriptor fd = write_event->fd();
    DEBUG("No.5 WebservSender::send() + fd=" + fd.to_string());
    ssize_t size = source->read(&p_data, MAX_READ_SIZE);
    if(size < 0){
        source->close();
        event->set_completed(true);
        return ;
    }

    p_data[size] = '\0';
    cout << "read data=" << p_data << endl;
    DEBUG("No.6 WebservSender::send()");
    while(size > 0)
    {
        DEBUG("No.7 WebservSender::send() + size=" + Utility::to_string(size));
        int result = writer->write(fd, p_data, size, NULL);
    //DEBUG("No.8 WebservSender::send() + p_data=" + p_data);
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
        size = source->read(&p_data, MAX_READ_SIZE);
        if(size < 0){
            source->close();
            event->set_completed(true);
            break;
        }

        p_data[size] = '\0';
        DEBUG("No.8 WebservSender::send() + p_data=" + Utility::to_string(p_data));
        if (result < 0){
        cout << "data=[" << p_data << "]" << endl;
        }
    }
    DEBUG("No.10 WebservSender::send()");

    //todo
    //event->set_next_flag(true);
    DEBUG("WebservSender::send() end");
}
