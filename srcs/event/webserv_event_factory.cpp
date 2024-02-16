#include "webserv_event.hpp"
#include "webserv_make_request_event.hpp"
#include "webserv_make_response_event.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"
#include "socket_reader.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_timeout_event.hpp"
#include "opened_socket_file.hpp"
#include "vector_file.hpp"
#include "webserv_io_event.hpp"
#include "webserv_io_socket_event.hpp"
#include "webserv_io_cgi_event.hpp"
#include "error_file.hpp"

WebservEventFactory::WebservEventFactory(
        Config *cfg,
        SocketController *socket_controller,
        FDManager *fd_manager,
        FileManager *file_manager,
        IOMultiplexing *io_multi_controller,
        EventManager *event_manager,
        IWriter *normal_writer,
        IWriter *socket_writer,
        IReader *normal_reader,
        IReader *socket_reader
        ) :
        cfg(cfg),
        socket_controller(socket_controller),
        fd_manager(fd_manager),
        file_manager(file_manager),
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
    DEBUG("WebservEventFactory::~WebservEventFactory Destructor");
}

void WebservEventFactory::register_file_manager(WebservEvent *event)
{
    file_manager->insert(event->entity()->fd(), event->entity()->io().source());
    file_manager->insert(event->entity()->fd(), event->entity()->io().destination());
}

void WebservEventFactory::make_and_push_write_cgi_event(FileDiscriptor pid, FileDiscriptor fd_out, Request *req)
{
    (void)req;
    MYINFO("make_and_push_write_cgi_event() pid=" + pid.to_string() + ", fd_out=" +  fd_out.to_string());
    this->fd_manager->add_socket_and_epoll_fd(pid, fd_out);
    this->io_multi_controller->add(fd_out, EPOLLOUT | EPOLLONESHOT);
}


WebservEvent *WebservEventFactory::make_keep_alive_event(WebservEvent *event)
{
    WebservEvent *new_event = WebservKeepAliveEvent::from_event(event);
    this->register_file_manager(new_event);
    return (new_event);
}



WebservEvent *WebservEventFactory::from_epoll_event(t_epoll_event const &event_epoll)
{
    DEBUG("WebservEventFactory::from_epoll_event():" + Utility::to_string(event_epoll.events));
    FileDiscriptor fd = FileDiscriptor::from_int(event_epoll.data.fd);
    FileDiscriptor io_fd;

    if(event_epoll.events & EPOLLRDHUP){
        DEBUG("WebservEvent::from_epoll_event: EPOLLRDHUP");
        WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);
        this->make_clean_event(cached_event, true);
    }else if(event_epoll.events & EPOLLIN){
        DEBUG("WebservEvent::from_epoll_event: EPOLLIN");
        if(this->fd_manager->is_registered(fd) == false)
        {
            MYINFO("WebservEvent::from_epoll_event() accept request fd:" + fd.to_string() + ",and new epoll_fd:" + io_fd.to_string());
            io_fd = this->socket_controller->accept_request(fd);
            this->fd_manager->add_socket_and_epoll_fd(io_fd, fd);
            this->io_multi_controller->add(io_fd, EPOLLIN);

            return (WebservNothingEvent::make_nothing_event());
        }else{
            WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);
            if(cached_event == NULL){
                FileDiscriptor sockfd = fd_manager->get_sockfd(fd);
                WebservEntity *entity = new WebservEntity(fd, sockfd, this->cfg);
                entity->config()->check();
                File *socket_io = OpenedSocketFile::from_fd(fd, socket_writer, socket_reader);
                File *read_dst = VectorFile::from_buf_size(MAX_STATUS_LINE);
                WebservEvent *event = WebservIOSocketEvent::as_read(fd, socket_io, read_dst, entity);
                this->cfg->check();
                event->entity()->config()->check();
                return (event);
            }else{
                cached_event->entity()->io().switching_io(EPOLLIN);
                cached_event->increase_timeout_count(-cached_event->timeout_count());
                return (cached_event);

            }
        }
    }else if(event_epoll.events & EPOLLOUT){
        DEBUG("WebservEvent::from_epoll_event: EPOLLOUT");
        WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);
        cached_event->entity()->io().switching_io(EPOLLOUT);
        return (cached_event);
    }else if(event_epoll.events & EPOLLERR){
        WARNING("Epoll event type is EPOLLERR");
        //todo
    }else if(event_epoll.events & EPOLLHUP){
        WARNING("Epoll event type is EPOLLHUP");
        //todo
    }else{
        WARNING("Epoll event type is undefined");
        throw std::runtime_error("Epoll event type is undefined");
    }
    return (NULL);
}

WebservEvent *WebservEventFactory::make_io_socket_for_cgi(WebservEvent *event, File *write_src, File *read_dst, ApplicationResult *result)
{
    DEBUG("WebservEventFactory::make_io_socket_for_cgi fd=" + event->entity()->fd().to_string());
    File *write_dst = OpenedSocketFile::from_fd(normal_writer, result->cgi_in());
    File *read_src = OpenedSocketFile::from_fd(normal_reader, result->cgi_out());
    FileDiscriptor socketfd = fd_manager->get_sockfd(event->entity()->fd());
    return (WebservIOCGIEvent::from_fd(result->cgi_in(), result->cgi_out(),  read_src, read_dst, write_src, write_dst, event));
}

WebservEvent *WebservEventFactory::make_io_socket_event_as_write(WebservEvent *event, File *src)
{
    DEBUG("WebservEventFactory::make_io_socket_event_as_write fd=" + event->entity()->fd().to_string());
    File *dst = OpenedSocketFile::from_fd(socket_writer, event->entity()->fd());
    return (WebservIOSocketEvent::as_write(event, event->entity()->fd(), src, dst));
}

WebservEvent *WebservEventFactory::make_io_socket_event_as_read(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_io_socket_event_as_read fd=" + event->entity()->fd().to_string());
    File *src = OpenedSocketFile::from_fd(socket_reader, event->entity()->fd());
    File *dst = VectorFile::from_buf_size(MAX_STATUS_LINE);
    return (WebservIOSocketEvent::as_read(event->entity()->fd(), src, dst, event->entity()));
}

WebservEvent *WebservEventFactory::make_making_request_event(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_making_request_event");
    WebservEvent *new_event = WebservMakeRequestEvent::from_event(event, event->entity()->io().destination(), NULL);

    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_making_response_event(WebservEvent *event, File *src)
{
    DEBUG("WebservEventFactory::make_making_response_event");
    WebservEvent *new_event = WebservMakeResponseEvent::from_event(event, src, NULL);

    return (new_event);
}

WebservEvent *WebservEventFactory::make_application_event(WebservEvent *event)
{
    WebservEvent *new_event;
    if(event->entity()->request()->is_cgi()){
        new_event = WebservApplicationWithCgiEvent::from_event(event);
    }else{
        new_event = WebservApplicationWithoutCgiEvent::from_event(event);
    }

    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_event_from_http_error(WebservEvent *event, char const *code_c)
{
    DEBUG("WebservEventFactory::make_event_from_http_error");
    Response const *res = event->entity()->response();
    if(res){
        delete res;
    }

    std::string code_str = code_c;
    StatusCode code;
    try{
        code = StatusCode::from_string(code_str);
    }catch (std::runtime_error &e){
        code = StatusCode::from_string("500");
    }

    File *dst = OpenedSocketFile::from_fd(socket_writer, event->entity()->fd());

    File *file = ErrorFile::from_status_code(code);
    if(event->entity()->app_result() != NULL){
        delete event->entity()->app_result();
    }
    ApplicationResult *result = ApplicationResult::from_status_code(code);
    result->set_file(file);
    event->entity()->set_result(result);

    return (WebservMakeResponseEvent::from_event(event, result, dst));
}


WebservEvent *WebservEventFactory::make_clean_event(WebservEvent *event, bool force_close)
{
    WebservEvent *new_event = WebservCleanEvent::from_webserv_event(event, force_close, fd_manager);

    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_timeout_event()
{
    return (WebservTimeoutEvent::make(this->fd_manager, this->event_manager));
}

