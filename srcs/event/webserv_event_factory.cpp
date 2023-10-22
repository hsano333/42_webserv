#include "webserv_event.hpp"
#include "webserv_read_event.hpp"
#include "webserv_write_event.hpp"
#include "webserv_application_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"
#include "socket_reader.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_clean_event.hpp"

WebservEventFactory::WebservEventFactory(
        SocketController *socket_controller,
        FDManager *fd_manager,
        IOMultiplexing *io_multi_controller,
        EventManager *event_manager,
        IWriter *normal_writer,
        IWriter *socket_writer,
        IReader *normal_reader,
        IReader *socket_reader
        ) :
        socket_controller(socket_controller),
        fd_manager(fd_manager),
        io_multi_controller(io_multi_controller),
        event_manager(event_manager),
        normal_writer(normal_writer),
        socket_writer(socket_writer),
        normal_reader(normal_reader),
        socket_reader(socket_reader)
{
;
}

WebservEventFactory::~WebservEventFactory()
{
    ;
}


WebservEvent *WebservEventFactory::from_epoll_event(t_epoll_event const &event_epoll)
{
    DEBUG("WebservEventFactory::from_epoll_event()");
    FileDiscriptor fd = FileDiscriptor::from_int(event_epoll.data.fd);
    FileDiscriptor io_fd;

    if(event_epoll.events & EPOLLIN){

        if(this->fd_manager->is_registered(fd) == false)
        {
            io_fd = this->socket_controller->accept_request(fd);
            MYINFO("WebservEvent::from_epoll_event() accept request fd:" + fd.to_string() + ",and new epoll_fd:" + io_fd.to_string());
            this->fd_manager->add_socket_and_epoll_fd(io_fd, fd);
            this->io_multi_controller->add(io_fd, EPOLLIN);

            return (new WebservNothingEvent);
        }else{
            //io_fd = this->fd_manager->socket_fd_from_epoll_fd(fd);
            DEBUG("WebservEvent::from_epoll_event: EPOLLIN");
            WebservEvent *cached_event = this->event_manager->get_event_waiting_reading(fd);
            if(cached_event == NULL){
                WebservEvent *event = WebservReadEvent::from_fd(fd);
                printf("event=%p\n", event);
                this->event_manager->add_event_waiting_reading(fd, event);
                return (event);
            }else{
                WebservEvent *new_event = this->make_read_event_from_event(cached_event);
                delete (cached_event);
                return (new_event);

            }
            return (cached_event);
        }
    }else if(event_epoll.events & EPOLLOUT){
        DEBUG("WebservEvent::from_epoll_event: EPOLLOUT");
        //FileDiscriptor io_fd = this->socket_controller->accept_request(fd);
        WebservEvent *cached_event = this->event_manager->get_event_waiting_writing(fd);
        //WebservWriteEvent *event = WebservWriteEvent::from_event(saved_event, this->socket_writer);
        //this->fd_manager->add_socket_and_epoll_fd(io_fd, fd);
        return (cached_event);
    }else{
        WARNING("Epoll event type is undefined");
        throw std::runtime_error("Epoll event type is undefined");

    }
    return (NULL);
}


WebservEvent *WebservEventFactory::make_read_event_from_event(WebservEvent *event)
{
    return (WebservReadEvent::from_event(event));
}

WebservEvent *WebservEventFactory::make_application_event(WebservEvent *event)
{
    return (WebservApplicationEvent::from_event(event));
}

WebservEvent *WebservEventFactory::make_write_event(WebservEvent *event, Response *res)
{
    return (WebservWriteEvent::from_event(event, res, socket_writer));
}

WebservEvent *WebservEventFactory::make_error_event(WebservEvent *event, StatusCode &code)
{
    return (WebservWriteEvent::from_status_code(event, code, socket_writer));
}


WebservEvent *WebservEventFactory::make_clean_event(WebservEvent *event, bool force_close)
{
    WebservCleanEvent *new_event = WebservCleanEvent::from_webserv_event(event, force_close);
    return (new_event);
}

