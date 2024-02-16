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
    if (event_manager->find(event->entity()->fd()) == false){
        DEBUG("EventController::restart_communication: add ");
        // 同じfdが登録されていないので、再度登録する。
        event_manager->add_event_waiting_epoll(event->entity()->fd(), event);

        //もし同じfdが登録されている場合、同じクライアントから、
        //再度HTTP通信が始まったことを意味するので、イベントを削除する。
        //つまり、再登録しない。
    }
}
*/

void EventController::change_write_event(WebservEvent *event)
{
    DEBUG("EventController::change_write_event() fd=" + event->entity()->fd().to_string());
    (void)event;
    this->io_multi_controller->modify(event->entity()->fd(), EPOLLIN | EPOLLONESHOT);

}

void EventController::set_next_epoll_event(WebservEvent *event, WebservEvent *next_event)
{
    E_EpollEvent next_epoll_event = event->get_next_epoll_event();
    MYINFO("EventController::next_epoll_event:" + Utility::to_string(next_epoll_event));

    if (next_epoll_event == EPOLL_READ){
        MYINFO("EventController::next is epoll read");
        this->io_multi_controller->modify(next_event->entity()->fd(), EPOLLIN | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->fd(), next_event);
    }else if (next_epoll_event == EPOLL_WRITE){
        MYINFO("EventController::next is epoll write");
        this->io_multi_controller->modify(next_event->entity()->fd(), EPOLLOUT | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->fd(), next_event);
    }else if (next_epoll_event == EPOLL_ADD_WRITE){
        MYINFO("EventController::next is epoll add writing");
        this->io_multi_controller->add(next_event->entity()->fd(), EPOLLOUT | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->fd(), next_event);
    }else if (next_epoll_event == EPOLL_FOR_CGI){
        MYINFO("EventController::next is epoll EPOLL_FOR_CGI");
        FileDiscriptor const &socket_fd = next_event->entity()->socket_fd();
        this->io_multi_controller->add(next_event->entity()->io().get_read_fd(), EPOLLIN | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->io().get_read_fd(), next_event);
        this->fd_manager->add_socket_and_epoll_fd(next_event->entity()->io().get_read_fd(), socket_fd);
    }else if (next_epoll_event == EPOLL_CLOSE){
        MYINFO("EventController::next is epoll closed");
        this->fd_manager->close_fd(event->entity()->fd());
    }else if(next_event){
        MYINFO("EventController::next is not epoll");
        this->event_manager->push(next_event);
    }
}

void EventController::next_event(WebservEvent *event)
{
    if(event->entity() == NULL){
        delete event;
        return;
    }
    WebservEvent *next_event;
    if(event->entity()->completed()){
        next_event = event->make_next_event(event, this->event_factory);
    }else{
        next_event = event;
    }
    set_next_epoll_event(event, next_event);
    if(event && next_event != event){
        delete event;
    }
}

