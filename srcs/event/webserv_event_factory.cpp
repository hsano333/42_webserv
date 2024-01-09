#include "webserv_event.hpp"
#include "webserv_read_event.hpp"
#include "webserv_make_request_event.hpp"
#include "webserv_write_event.hpp"
#include "webserv_application_event.hpp"
#include "webserv_nothing_event.hpp"
#include "webserv_make_request_event.hpp"
#include "global.hpp"
#include "socket_reader.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_clean_event.hpp"

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
    file_manager->insert(event->fd(), event->src());
    file_manager->insert(event->fd(), event->dst());
}

void WebservEventFactory::make_and_push_write_cgi_event(FileDiscriptor pid, FileDiscriptor fd_out, Request *req)
{
    MYINFO("make_and_push_write_cgi_event() pid=" + pid.to_string() + ", fd_out=" +  fd_out.to_string());
    this->fd_manager->add_socket_and_epoll_fd(pid, fd_out);
    this->io_multi_controller->add(fd_out, EPOLLOUT | EPOLLONESHOT);
    WebservEvent *write_event = WebservWriteEvent::from_cgi_fd(fd_out, req, socket_reader,normal_writer);
    this->event_manager->push(write_event);

    this->register_file_manager(write_event);
}

void WebservEventFactory::make_and_push_read_cgi_event(FileDiscriptor pid, FileDiscriptor fd_in)
{
    MYINFO("make_and_push_read_cgi_event() pid=" + pid.to_string() + ", fd_in=" + fd_in.to_string());
    //this->fd_manager->add_socket_and_epoll_fd(pid, fd_in);
    //this->io_multi_controller->add(fd_in, EPOLLIN | EPOLLONESHOT);
    WebservEvent *read_event = WebservReadEvent::from_cgi_fd(fd_in, pid, normal_reader);
    this->event_manager->push(read_event);


    this->register_file_manager(read_event);
}


WebservEvent *WebservEventFactory::make_nothing_event(FileDiscriptor fd, FileDiscriptor sock_fd)
{
    MYINFO("make_nothing_event() pid=" + fd.to_string() + ", fd_out=" + sock_fd.to_string());
    this->fd_manager->add_socket_and_epoll_fd(fd, sock_fd);
    this->io_multi_controller->add(fd, EPOLLIN | EPOLLONESHOT);
    WebservEvent *new_event = new WebservNothingEvent(fd);


    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_nothing_event(FileDiscriptor fd)
{
    MYINFO("make_nothing_event() fd=" + fd.to_string());
    //FileDiscriptor sock_fd = this->fd_manager->get_sockfd(fd);
    //this->fd_manager->add_socket_and_epoll_fd(fd, sock_fd);
    this->io_multi_controller->modify(fd, EPOLLIN | EPOLLONESHOT);
    WebservEvent *new_event = new WebservNothingEvent(fd);
    //return (this->make_nothing_event(fd, sock_fd));
    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::from_epoll_event(t_epoll_event const &event_epoll)
{
    DEBUG("WebservEventFactory::from_epoll_event()");
    FileDiscriptor fd = FileDiscriptor::from_int(event_epoll.data.fd);
    FileDiscriptor io_fd;

    if(event_epoll.events & EPOLLRDHUP){
        DEBUG("EPOLLRDHUP: ");

        WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);
        this->make_clean_event(cached_event, true);
    }else if(event_epoll.events & EPOLLIN){

        if(this->fd_manager->is_registered(fd) == false)
        {
            MYINFO("WebservEvent::from_epoll_event() accept request fd:" + fd.to_string() + ",and new epoll_fd:" + io_fd.to_string());
            io_fd = this->socket_controller->accept_request(fd);
            this->fd_manager->add_socket_and_epoll_fd(io_fd, fd);
            this->io_multi_controller->add(io_fd, EPOLLIN);

            return (new WebservNothingEvent(io_fd));
        }else{
            //sock_fd = this->fd_manager->socket_fd_from_epoll_fd(fd);
            DEBUG("WebservEvent::from_epoll_event: EPOLLIN");
            WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);
            if(cached_event == NULL){
                MYINFO("cached_event is NULL");
                FileDiscriptor sockfd = fd_manager->get_sockfd(fd);
                WebservEvent *event = WebservReadEvent::from_fd(fd, sockfd, socket_reader);
                //printf("event=%p\n", event);
                //this->event_manager->add_event_waiting_epoll(fd, event);
                return (event);
            }else{

                /*
                if (!(fd == cached_event->fd()) && cached_event->cgi_event().is_cgi() && (fd == cached_event->cgi_event().cgi_fd()))
                {
                    MYINFO("test CGI No.1");
                    cached_event->cgi_event().add_cgi_triger(CGI_READ);
                    if ( cached_event->cgi_event().cgi_triger() > CGI_READ){
                    MYINFO("test CGI No.2");
                        return cached_event;
                    }else{
                    MYINFO("test CGI No.3");
                        return (new WebservNothingEvent);
                    }
                    //登録したfdがcgi用のイベントであるとき、
                }else if(cached_event->cgi_event().is_cgi()){
                    MYINFO("test CGI No.4");

                }
                */
                // reset timeout
                cached_event->increase_timeout_count(-cached_event->timeout_count());
                MYINFO("cached_event exists");
                //WebservEvent *new_event = WebservReadEvent::from_fd(fd);
                //MYINFO("delete cached_event");
                //delete (cached_event);
                //MYINFO("deleted cached_event");
                //this->event_manager->add_event_waiting_epoll(fd, new_event);
                return (cached_event);
                //return (cached_event);

            }
            //return (cached_event);
        }
    }else if(event_epoll.events & EPOLLOUT){

        DEBUG("WebservEvent::from_epoll_event: EPOLLOUT");
        WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);

        /*
        if (!(fd == cached_event->fd()) && cached_event->cgi_event().is_cgi() && fd == cached_event->cgi_event().cgi_fd())
        {
                    MYINFO("test CGI No.6");
            //cached_event->cgi_event().add_cgi_triger(CGI_Triger.CGI_READ);
            //登録したfdがcgi用のイベントであるとき、
        }else if(cached_event->cgi_event().is_cgi()){
                    MYINFO("test CGI No.7");
            cached_event->cgi_event().add_cgi_triger(CGI_WAIT);
            if ( cached_event->cgi_event().cgi_triger() > CGI_READ){
                    MYINFO("test CGI No.8");
                return cached_event;
            }else{
                    MYINFO("test CGI No.9");
                return (new WebservNothingEvent);
            }

                    MYINFO("test CGI No.10");


            //登録したfdがcgi用のイベントではないが、CGIは実行されていたとき
        }
        */


        //FileDiscriptor sock_fd = this->socket_controller->accept_request(fd);
        //WebservWriteEvent *event = WebservWriteEvent::from_event(saved_event, this->socket_writer);
        //this->fd_manager->add_socket_and_epoll_fd(sock_fd, fd);
        return (cached_event);
    }else{
        WARNING("Epoll event type is undefined");
        throw std::runtime_error("Epoll event type is undefined");

    }
    return (NULL);
}


WebservEvent *WebservEventFactory::make_read_event_from_event(WebservEvent *event)
{
    FileDiscriptor sockfd = fd_manager->get_sockfd(event->fd());
    WebservEvent *new_event =  WebservReadEvent::from_event(event, sockfd, socket_reader);

    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_making_request_event(WebservEvent *event)
{
    WebservEvent *new_event = WebservMakeRequestEvent::from_event(event, this->socket_reader, this->cfg);

    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_application_event(WebservEvent *event)
{
    WebservEvent *new_event = WebservApplicationEvent::from_event(event);

    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_write_event_for_cgi(WebservEvent *event, Response *res)
{
    WebservEvent *new_event = WebservWriteEvent::from_event_for_cgi(event, res, socket_writer);

    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_write_event(WebservEvent *event, Response *res)
{
    WebservEvent *new_event = WebservWriteEvent::from_event(event, res, socket_writer);

    this->register_file_manager(new_event);
    return (new_event);
}

WebservEvent *WebservEventFactory::make_error_event(WebservEvent *event, char const *code_c)
{
    DEBUG("WebservEventFactory::make_error_event");
    File *file = NULL;
    Request *req = event->req();

    std::string code_str = code_c;
    StatusCode code;
    try{
        code = StatusCode::from_string(code_str);
    }catch (std::runtime_error &e){
        code = StatusCode::from_string("500");
    }

    if (req){
        file = cfg->get_error_file(req, code);
        if (!file || file->can_read() == false){
            delete file;
            file = NULL;
        }
    }
    return (WebservWriteEvent::from_error_status_code(event, code, file, socket_writer));
}


WebservEvent *WebservEventFactory::make_clean_event(WebservEvent *event, bool force_close)
{
    WebservCleanEvent *new_event = WebservCleanEvent::from_webserv_event(event, force_close);

    this->register_file_manager(new_event);
    return (new_event);
}

