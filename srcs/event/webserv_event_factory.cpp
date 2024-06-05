#include "webserv_event.hpp"
#include "webserv_make_request_event.hpp"
#include "webserv_make_response_event.hpp"
#include "webserv_application_event.hpp"
#include "webserv_application_upload_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"
#include "socket_reader.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_clean_event.hpp"
#include "webserv_timeout_event.hpp"
#include "socket_file.hpp"
#include "vector_file.hpp"
#include "webserv_io_event.hpp"
#include "webserv_io_socket_event.hpp"
#include "webserv_io_get_cgi_event.hpp"
#include "webserv_io_post_cgi_event.hpp"
#include "error_file.hpp"
#include "webserv_keep_alive_event.hpp"
#include "webserv_waiting_get_cgi_event.hpp"
#include "webserv_waiting_post_cgi_event.hpp"
#include "webserv_make_response_for_get_cgi_event.hpp"
#include "webserv_make_response_for_post_cgi_event.hpp"
#include "webserv_error_event.hpp"

WebservEventFactory::WebservEventFactory(
        Config *cfg,
        WebservFileFactory *file_factory,
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
        file_factory(file_factory),
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

WebservEvent *WebservEventFactory::register_event(WebservEvent *event)
{
    this->event_manager->add_events_will_deleted(event->entity()->fd(), event);
    return (event);
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
    DEBUG("WebservEventFactory::make_keep_alive_event");
    WebservEvent *new_event = WebservKeepAliveEvent::from_event(event);


    // keep_aliveのみ削除しない
    //this->register_event(new_event);
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
            try{
                io_fd = this->socket_controller->accept_request(fd);
            }catch(std::invalid_argument &e){
                return (NULL);
            }
            MYINFO("WebservEvent::from_epoll_event() accept request fd:" + fd.to_string() + ",and new epoll_fd:" + io_fd.to_string());
            this->fd_manager->add_socket_and_epoll_fd(io_fd, fd);
            //this->event_manager->add_event_waiting_epoll(fd, NULL);
            this->io_multi_controller->add(io_fd, EPOLLIN  | EPOLLONESHOT );

            return (NULL);
        }else{
            MYINFO("WebservEvent::from_epoll_event() fd:" + fd.to_string() + " is registred");
            WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);
            if(cached_event == NULL || cached_event->which() == KEEP_ALIVE_EVENT){
                //if(cached_event && cached_event->which() == KEEP_ALIVE_EVENT){
                if(cached_event){
                    MYINFO("WebservEvent::from_epoll_event() delete keep alive event");
                    delete cached_event->entity();
                    delete cached_event;
                }

                FileDiscriptor sockfd = fd_manager->get_sockfd(fd);
                WebservEntity *entity;
                try{
                    entity = new WebservEntity(fd, sockfd, this->cfg);
                }catch(...){

                }
                DEBUG("new entity address:" + Utility::to_string(entity));
                entity->config()->check();
                FileDiscriptor const &fd_ref = entity->fd();
                WebservFile *socket_io = this->file_factory->make_socket_file(fd_ref, fd_ref, socket_writer, socket_reader);
                WebservFile *read_dst = this->file_factory->make_vector_file_for_socket(fd_ref, MAX_REAUEST_EXCEPT_BODY);
                WebservEvent *event = WebservIOSocketEvent::as_read(fd_ref, socket_io, read_dst, entity);
                this->cfg->check();
                event->entity()->config()->check();

                this->register_event(event);
                return (event);

            }else{
                cached_event->entity()->io().switching_io(EPOLLIN);
                cached_event->update_time();
                return (cached_event);

            }
        }
    }else if(event_epoll.events & EPOLLOUT){
        MYINFO("WebservEvent::from_epoll_event() EPOLLOUT fd:" + fd.to_string());
        WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);
        if(cached_event){
            cached_event->entity()->io().switching_io(EPOLLOUT);
            cached_event->update_time();
        }
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

WebservEvent *WebservEventFactory::make_waiting_socket_out_cgi(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_waiting_out_cgi fd=" + event->entity()->fd().to_string());
    WebservEvent *new_event = WebservWaitingGetCGIEvent::from_event(event);

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_waiting_get_cgi(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_waiting_out_cgi fd=" + event->entity()->fd().to_string());
    WebservEvent *new_event = WebservWaitingGetCGIEvent::from_event(event);

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_waiting_post_cgi(WebservEvent *event)
{
    WebservEvent *new_event = WebservWaitingPostCGIEvent::from_event(event);

    this->register_event(new_event);
    return (new_event);
}

/*
WebservEvent *WebservEventFactory::make_waiting_cgi(WebservEvent *event, WebservFile *write_dst, WebservFile *read_src, ApplicationResult *result)
{
    DEBUG("WebservEventFactory::make_waiting_cgi fd=" + event->entity()->fd().to_string());
    WebservFile *read_dst = this->file_factory->make_socket_file(result->cgi_in(), normal_writer, NULL);
    WebservFile *write_src = this->file_factory->make_socket_file(result->cgi_out(), NULL, normal_reader);
    FileDiscriptor socketfd = fd_manager->get_sockfd(event->entity()->fd());

    WebservEvent *new_event = WebservWaitingPostCGIEvent::from_fd(result->cgi_in(), result->cgi_out(),  read_src, read_dst, write_src, write_dst, event);

    return (new_event);
}
*/

/*
WebservEvent *WebservEventFactory::make_waiting_out_cgi(WebservEvent *event, WebservFile *write_src, WebservFile *read_dst, ApplicationResult *result)
{
    DEBUG("WebservEventFactory::make_waiting_out_cgi fd=" + event->entity()->fd().to_string());
    //WebservFile *write_dst = this->file_factory->make_socket_file(result->cgi_in(), normal_writer, NULL);
    //WebservFile *read_src = this->file_factory->make_socket_file(result->cgi_out(), NULL, normal_reader);
    FileDiscriptor socketfd = fd_manager->get_sockfd(event->entity()->fd());

    WebservEvent *new_event = WebservWaitingGetCGIEvent::from_fd(result->cgi_in(), result->cgi_out(),  read_src, read_dst, write_src, write_dst, event);

    return (new_event);
}
*/

WebservEvent *WebservEventFactory::make_io_socket_for_post_cgi(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_io_socket_for_post_cgi fd=" + event->entity()->fd().to_string());
    WebservEvent *new_event = WebservIOPostCGIEvent::from_event(event);
    DEBUG("new_event address:" + Utility::to_string(new_event));

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_io_socket_for_get_cgi(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_io_socket_for_get_cgi fd=" + event->entity()->fd().to_string());
    WebservEvent *new_event = WebservIOGetCGIEvent::from_event(event);

    this->register_event(new_event);
    return (new_event);
}


WebservEvent *WebservEventFactory::make_io_socket_event_as_write(WebservEvent *event, WebservFile *src)
{
    DEBUG("WebservEventFactory::make_io_socket_event_as_write fd=" + event->entity()->fd().to_string());
    WebservFile *file = this->file_factory->make_socket_file(event->entity()->fd(), event->entity()->fd(), socket_writer, NULL);
    if(src->is_chunk()){
        DEBUG("WebservEventFactory:: Chunked");
        WebservEvent *new_event = WebservIOSocketEvent::as_chunked_write(event, event->entity()->fd(), src, file);
        this->register_event(new_event);
        return (new_event);
    }
    WebservEvent *new_event = WebservIOSocketEvent::as_write(event, event->entity()->fd(), src, file);

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_io_socket_event_as_read(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_io_socket_event_as_read fd=" + event->entity()->fd().to_string());
    WebservFile *src = this->file_factory->make_socket_file(event->entity()->fd(), event->entity()->fd(), NULL, socket_reader);
    WebservFile *dst = this->file_factory->make_vector_file_for_socket(event->entity()->fd(), MAX_REAUEST_EXCEPT_BODY);
    WebservEvent *new_event = WebservIOSocketEvent::as_read(event->entity()->fd(), src, dst, event->entity());

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_io_socket_event_as_read(WebservEvent *event, WebservFile *src)
{
    DEBUG("WebservEventFactory::make_io_socket_event_as_read fd=" + event->entity()->fd().to_string());
    WebservFile *dst = this->file_factory->make_vector_file_for_socket(event->entity()->fd(), MAX_REAUEST_EXCEPT_BODY);
    WebservEvent *new_event = WebservIOSocketEvent::as_read(event->entity()->fd(), src, dst, event->entity());

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_making_request_event(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_making_request_event");
    WebservEvent *new_event = WebservMakeRequestEvent::from_event(event, event->entity()->io().destination(), NULL);

    this->register_event(new_event);
    return (new_event);
}



WebservEvent *WebservEventFactory::make_making_response_for_get_cgi_event(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_making_response_for_get_cgi_event");
    WebservEvent *new_event = WebservMakeResponseForGetCGIEvent::from_event(event);

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_making_response_for_post_cgi_event(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_making_response_for_post_cgi_event");
    WebservEvent *new_event = WebservMakeResponseForPostCGIEvent::from_event(event);

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_making_response_event(WebservEvent *event, WebservFile *src, WebservFile *dst)
{
    DEBUG("WebservEventFactory::make_making_response_event");
    WebservEvent *new_event = WebservMakeResponseEvent::from_event(event, src, dst);

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_application_event(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_application_event()");
    WebservEvent *new_event;
    new_event = WebservApplicationEvent::from_event(event);

    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_making_upload_event(WebservEvent *event, WebservFile *src)
{
    DEBUG("WebservEventFactory::make_making_upload_event");
    std::string const &filepath = event->entity()->request()->requested_path();
    std::string const &boundary = event->entity()->body().boundary();
    if(event->entity()->request()->is_directory() == false){
        ERROR("speciied path is not directory: " + filepath);
        throw HttpException("404");
    }

    WebservFile *dst = this->file_factory->make_multi_normal_file(filepath, boundary, event->entity()->fd());
    WebservEvent *new_event = WebservApplicationUploadEvent::from_event(event, src, dst);
    ApplicationResult *result = event->entity()->app_result();
    result->set_file(dst);

    this->register_event(new_event);
    return (new_event);
}


WebservEvent *WebservEventFactory::make_event_from_http_error(WebservEvent *event, char const *code_c)
{
    DEBUG("WebservEventFactory::make_event_from_http_error");
    /*
    Response const *current_res = event->entity()->response();
    if(current_res){
        delete current_res;
    }

    std::string code_str = code_c;
    StatusCode code;
    try{
        code = StatusCode::from_string(code_str);
    }catch (std::runtime_error &e){
        code = StatusCode::from_string("500");
    }

    WebservFile *dst = this->file_factory->make_socket_file(event->entity()->fd(), event->entity()->fd(), socket_writer, NULL);

    WebservFile *file = this->file_factory->make_error_file(event->entity()->fd(), code);
    if(event->entity()->app_result() != NULL){
        delete event->entity()->app_result();
    }
    ApplicationResult *result = ApplicationResult::from_status_code(code, "NONE");
    result->set_file(file);
    event->entity()->set_result(result);

    WebservFile *result_file = this->file_factory->make_webserv_file_regular(event->entity()->fd(), result);
    WebservEvent *new_event = WebservMakeResponseEvent::from_event(event, result_file, dst);
    */

    std::string code_str = code_c;
    StatusCode code;
    try{
        code = StatusCode::from_string(code_str);
    }catch (std::runtime_error &e){
        code = StatusCode::from_string("500");
    }
    WebservEvent *new_event = WebservErrorEvent::from_event(event, code);
    this->register_event(new_event);
    return (new_event);
}


WebservEvent *WebservEventFactory::make_clean_event(WebservEvent *event, bool force_close)
{
    DEBUG("WebservEventFactory::make_clean_event()");
    WebservEvent *new_event = WebservCleanEvent::from_event(event, force_close);
    this->register_event(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_timeout_event(WebservEvent *event)
{
    DEBUG("WebservEventFactory::make_timeout_event()");
    WebservEvent *new_event = (WebservTimeoutEvent::make(event));
    this->register_event(new_event);
    return (new_event);
}

