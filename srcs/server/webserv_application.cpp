
#include "webserv_application.hpp"

WebservApplication::WebservApplication(
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

WebservApplication::~WebservApplication()
{
    ;
}

void WebservApplication::execute(WebservEvent *event)
{
    DEBUG("WebservApplication::execute");
    (void)event;
    //Response *res = event->res;
    cout << "delete event" << endl;
    //FileDiscriptor socket_fd = this->fd_manager->socket_fd_from_epoll_fd(event->get_fd());
    //this->fd_manager->close_fd(event->get_fd());
    io_multi_controller->modify(event->get_fd(), EPOLLOUT);
    //io_multi_controller->modify(event->get_fd(), EPOLLOUT);
    delete (event);
    //WebservEvent *next_event = event_factory->from_response(res);
    //event_manager.push(next_event);
}
