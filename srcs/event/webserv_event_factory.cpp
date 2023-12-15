#include "webserv_event.hpp"
#include "webserv_read_event.hpp"
#include "webserv_parser_event.hpp"
#include "webserv_write_event.hpp"
#include "webserv_application_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"
#include "socket_reader.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_clean_event.hpp"

WebservEventFactory::WebservEventFactory(
        Config *cfg,
        SocketController *socket_controller,
        FDManager *fd_manager,
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
            io_fd = this->socket_controller->accept_request(fd);
            MYINFO("WebservEvent::from_epoll_event() accept request fd:" + fd.to_string() + ",and new epoll_fd:" + io_fd.to_string());
            this->fd_manager->add_socket_and_epoll_fd(io_fd, fd);
            this->io_multi_controller->add(io_fd, EPOLLIN);

            return (new WebservNothingEvent);
        }else{
            //io_fd = this->fd_manager->socket_fd_from_epoll_fd(fd);
            DEBUG("WebservEvent::from_epoll_event: EPOLLIN");
            WebservEvent *cached_event = this->event_manager->pop_event_waiting_epoll(fd);
            if(cached_event == NULL){
                MYINFO("cached_event is NULL");
                FileDiscriptor sockfd = fd_manager->get_sockfd(fd);
                WebservEvent *event = WebservReadEvent::from_fd(fd, sockfd);
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


        //FileDiscriptor io_fd = this->socket_controller->accept_request(fd);
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
    FileDiscriptor sockfd = fd_manager->get_sockfd(event->fd());
    return (WebservReadEvent::from_event(event, sockfd));
}

WebservEvent *WebservEventFactory::make_parser_event(WebservEvent *event)
{
    return (WebservParserEvent::from_event(event));
}

WebservEvent *WebservEventFactory::make_application_event(WebservEvent *event)
{
    return (WebservApplicationEvent::from_event(event));
}

WebservEvent *WebservEventFactory::make_write_event(WebservEvent *event, Response *res)
{
    return (WebservWriteEvent::from_event(event, res, socket_writer));
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
    return (new_event);
}

