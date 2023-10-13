
#include "webserv_executer.hpp"
#include "request.hpp"

WebservExecuter::WebservExecuter(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager,
            FDManager *fd_manager,
            Config *cfg
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager),
        fd_manager(fd_manager),
        cfg(cfg)
{
    ;
}

WebservExecuter::~WebservExecuter()
{
    ;
}

std::string identify_path(URI &uri)
{
    (void)uri;
    return ("test");
}

void WebservExecuter::make_application(WebservApplicationEvent *event)
{
    Request *req = event->req();
    cout << "Webserv Executer No.1" << endl;
    req->print_info();
    //cgi

}

void WebservExecuter::execute(WebservEvent *event)
{
    DEBUG("WebservExecuter::execute");
    WebservApplicationEvent *app_event = static_cast<WebservApplicationEvent*>(event);
    this->make_application(app_event);

    //is cgi?
    //which get,post, delete
    // target file

    //(void)event;
    //Response *res = event->res;
    //cout << "delete event" << endl;
    //FileDiscriptor socket_fd = this->fd_manager->socket_fd_from_epoll_fd(event->get_fd());
    //this->fd_manager->close_fd(event->get_fd());
    //io_multi_controller->modify(event->get_fd(), EPOLLOUT);
    //io_multi_controller->modify(event->get_fd(), EPOLLOUT);
    //delete (event);
    //WebservEvent *next_event = event_factory->from_response(res);
    //event_manager.push(next_event);
}
