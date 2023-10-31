#include "event_controller.hpp"
#include "event_controller.hpp"

EventController::EventController(
            EventManager *event_manager,
            IOMultiplexing *io_multi_controller,
            WebservEventFactory *event_factory
        )
    :
        event_manager(event_manager),
        io_multi_controller(io_multi_controller),
        event_factory(event_factory)
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
    DEBUG("EventController::change_write_event()");
    (void)event;
    this->io_multi_controller->modify(event->fd(), EPOLLIN);

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
    }else{
        next_event = event;
    }
    printf("next event=%p\n", next_event);
    E_EpollEvent next_epoll_event = event->get_next_epoll_event();

    if (next_epoll_event == EPOLL_READ){
        this->io_multi_controller->modify(event->fd(), EPOLLIN);
        this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);
        MYINFO("EventController::next is epoll read");

    }else if (next_epoll_event == EPOLL_WRITE){
        MYINFO("EventController::next is epoll write");
        this->io_multi_controller->modify(event->fd(), EPOLLOUT);
        //this->event_manager->erase_event_waiting_epoll(event->fd());
        this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);
    //}else if (next_epoll_event == EPOLL_CLOSE){
        //this->fd_manager->close_fd(app_event->fd());
    }else if(next_event){
        MYINFO("EventController::next is not epoll");
        event_manager->push(next_event);
    }

    //if (next_epoll_event != EPOLL_CONTINUE){
    if(event && next_event != event){
        delete event;
    }
    //}
}

