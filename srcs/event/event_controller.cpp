#include "event_controller.hpp"
#include "event_controller.hpp"

EventController::EventController(
            EventManager *event_manager,
            IOMultiplexing *io_multi_controller,
            WebservEventFactory *event_factory,
            FDManager *fd_manager
        )
    :
        event_manager(event_manager),
        io_multi_controller(io_multi_controller),
        event_factory(event_factory),
        fd_manager(fd_manager)
{
;
}

EventController::~EventController()
{
;
}

/*
void EventController::restart_communication(WebservEvent *event)
{
    (void)event;
    if (event_manager->find(event->fd()) == false){
        DEBUG("EventController::restart_communication: add ");
        // 同じfdが登録されていないので、再度登録する。
        event_manager->add_event_waiting_epoll(event->fd(), event);

        //もし同じfdが登録されている場合、同じクライアントから、
        //再度HTTP通信が始まったことを意味するので、イベントを削除する。
        //つまり、再登録しない。
    }
}
*/

void EventController::change_write_event(WebservEvent *event)
{
    DEBUG("EventController::change_write_event() fd=" + event->fd().to_string());
    (void)event;
    this->io_multi_controller->modify(event->fd(), EPOLLIN | EPOLLONESHOT);

}

void EventController::set_next_epoll_event(WebservEvent *event, WebservEvent *next_event)
{
    E_EpollEvent next_epoll_event = event->get_next_epoll_event();
    MYINFO("EventController::next_epoll_event:" + Utility::to_string(next_epoll_event));

    if (next_epoll_event == EPOLL_READ){
        this->io_multi_controller->modify(next_event->fd(), EPOLLIN | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);
        MYINFO("EventController::next is epoll read fd=" + event->fd().to_string());
        MYINFO("EventController::next is epoll read next fd=" + next_event->fd().to_string());
    }else if (next_epoll_event == EPOLL_WRITE){
        MYINFO("EventController::next is epoll write");
        this->io_multi_controller->modify(next_event->fd(), EPOLLOUT | EPOLLONESHOT);
        //this->event_manager->erase_event_waiting_epoll(event->fd());
        this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);
        MYINFO("EventController::next is epoll write fd=" + event->fd().to_string());
        MYINFO("EventController::next is epoll write next fd=" + next_event->fd().to_string());
    }else if (next_epoll_event == EPOLL_ADD_WRITE){
        MYINFO("EventController::next is epoll add write");
        this->io_multi_controller->add(next_event->fd(), EPOLLOUT | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);
    }else if (next_epoll_event == EPOLL_CGI){

        this->io_multi_controller->add(next_event->fd(), EPOLLOUT | EPOLLONESHOT);
        this->io_multi_controller->add(next_event->fd(), EPOLLIN | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);

        /*
    }else if (next_epoll_event == EPOLL_CGI_IN){
        MYINFO("EPOLL_CGI_IN:" + Utility::to_string(event->cgi_event().cgi_fd()));
        this->io_multi_controller->modify(event->fd(), EPOLLOUT | EPOLLONESHOT);
        this->io_multi_controller->modify(event->cgi_event().cgi_fd(), EPOLLOUT | EPOLLONESHOT);
        //this->fd_manager->close_fd(app_event->fd());
    }else if (next_epoll_event == EPOLL_CGI_OUT){
        MYINFO("EPOLL_CGI_OUT:" + Utility::to_string(event->cgi_event().cgi_fd()));
        this->io_multi_controller->modify(event->fd(), EPOLLOUT | EPOLLONESHOT);
        MYINFO("EPOLL_CGI_OUT No.1:" + Utility::to_string(event->cgi_event().cgi_fd()));
        this->io_multi_controller->modify(event->cgi_event().cgi_fd(), EPOLLIN | EPOLLONESHOT);
        MYINFO("EPOLL_CGI_OUT No.2:" + Utility::to_string(event->cgi_event().cgi_fd()));
        //this->fd_manager->close_fd(app_event->fd());
        */
    //}else if (next_epoll_event == EPOLL_CGI_STOP){
        //this->io_multi_controller->modify(event->fd(), EPOLLIN | EPOLLONESHOT);
    }else if (next_epoll_event == EPOLL_CLOSE){
        this->fd_manager->close_fd(event->fd());
        //DEBUG("EventController::next_event No.1 delete event:" + Utility::to_string(event));
        //delete event;
        //event = NULL;
    }else if(next_event){
        MYINFO("EventController::next is not epoll");
        this->event_manager->push(next_event);
    }
}

void EventController::next_event(WebservEvent *event)
{
    DEBUG("EventController::next_event() No.0");
    printf("event=%p\n", event);
    //if (event->is_completed() == false){
        //return ;
    //}
    DEBUG("EventController::next_event()");
    WebservEvent *next_event;
    if(event->is_completed()){
        next_event = event->make_next_event(event, this->event_factory);
        if(next_event){
            MYINFO("next_event=" + Utility::to_string(next_event->which()));
        }
    }else{
        next_event = event;
    }
    printf("next event=%p\n", next_event);

    set_next_epoll_event(event, next_event);
    //if (next_epoll_event != EPOLL_CONTINUE){
    if(event && next_event != event){
        //MYINFO("EventController::next_event() delete current event");
        DEBUG("EventController::next_event No.2 delete event:" + Utility::to_string(event));
        delete event;
    }
}

