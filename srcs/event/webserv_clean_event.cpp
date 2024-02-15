
#include "webserv_clean_event.hpp"
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservCleanEvent::WebservCleanEvent()
{
    ;
}

WebservCleanEvent::~WebservCleanEvent()
{
    ;
}

EWebservEvent WebservCleanEvent::which()
{
    return (CLEAN_EVENT);
}

WebservEvent* WebservCleanEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    if (event->entity()->force_close()){
        MYINFO("WebservCleanEvent::make_next_event() NULL");
        //delete this;
        return (NULL);
    }else{
        MYINFO("WebservCleanEvent::make_next_event() Read Event");
        printf("WebservCleanEvent::make_next_event event=%p\n", event);
        //sock_fd = this->socket_controller->accept_request(fd);
        return (event_factory->make_keep_alive_event(event ));
        //return (event_factory->make_read_event_from_event(event));
        //this->fd_manager->add_socket_and_epoll_fd(io_fd, fd);
        //this->io_multi_controller->add(io_fd, EPOLLIN);
    }
    (void)event_factory;
    (void)event;
    return (NULL);
}

E_EpollEvent WebservCleanEvent::get_next_epoll_event(WebservEvent *event)
{
    if (event->entity()->force_close()){
        return (EPOLL_CLOSE);
    }else{
        return (EPOLL_READ);
    }
}

/*
FileDiscriptor &WebservCleanEvent::fd()
{
    return (this->fd_);
}

Request *WebservCleanEvent::req()
{
    return (this->req_);
}

Response *WebservCleanEvent::res()
{
    return (this->res_);
}
*/
/*

File *WebservCleanEvent::src()
{
    return (this->source_file);
}

File *WebservCleanEvent::dst()
{
    return (this->destination_file);
}

void WebservCleanEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservCleanEvent::set_dst(File *file)
{
    this->destination_file = file;
}
*/

/*
void WebservCleanEvent::set_null_res_and_req()
{
    //this->res_ = NULL;
    //this->req_ = NULL;
}
*/

/*
bool WebservCleanEvent::is_completed()
{
    return (this->is_completed_);
}

void WebservCleanEvent::set_force_close(bool flag)
{
    this->force_close = flag;
}

void WebservCleanEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}
*/

/*
void WebservCleanEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservCleanEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}
*/

/*
int WebservCleanEvent::timeout_count()
{
    return (this->timeout_count_);
}

bool WebservCleanEvent::is_force_close()
{
    return (this->force_close);
}
*/

WebservCleanEvent *WebservCleanEvent::singleton = NULL;
WebservCleanEvent *WebservCleanEvent::get_instance(FDManager* fd_manager)
{
    if (WebservCleanEvent::singleton == NULL){
        singleton = new WebservCleanEvent();
        singleton->fd_manager = fd_manager;
    }
    return (singleton);
}

#include "webserv_cleaner.hpp"
WebservEvent *WebservCleanEvent::from_webserv_event(WebservEvent *event, bool force_close, FDManager *fd_manager)
{
    DEBUG("WebservCleanEvent::from_webserv_event");
    WebservCleanEvent *clean_event = WebservCleanEvent::get_instance(fd_manager);
    WebservEvent *new_event =  new WebservEvent(clean_event, clean, event->entity());
    new_event->entity()->set_force_close(force_close);
    new_event->entity()->io().set_source(event->entity()->request());
    return (new_event);
}


void WebservCleanEvent::close_fd(FileDiscriptor const &fd)
{
    this->fd_manager->close_fd(fd);
}
/*
void WebservCleanEvent::clean_res_and_req()
{
    //delete this->res_;
    //delete this->req_;

    this->res_ = NULL;
    this->req_ = NULL;

}
*/

/*
void WebservCleanEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservCleanEvent::cgi_event()
{
    return (this->cgi_event_);
}

WebservEntity*WebservCleanEvent::entity()
{
    return (this->entity_);
}
*/
