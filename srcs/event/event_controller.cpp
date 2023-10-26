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

void EventController::restart_communication(WebservEvent *event)
{
    (void)event;
    if (event_manager->find(event->fd()) == false){
        // 同じfdが登録されていないので、再度登録する。
        event_manager->add_event_waiting_reading(event->fd(), event);

        //もし同じfdが登録されている場合、同じクライアントから、
        //再度HTTP通信が始まったことを意味するので、イベントを削除する。
        //つまり、再登録しない。
    }
}

void EventController::change_write_event(WebservEvent *event)
{
    DEBUG("EventController::change_write_event()");
    (void)event;
    this->io_multi_controller->modify(event->fd(), EPOLLIN);

}

void EventController::next_event(WebservEvent *event)
{
    cout << "next_event No.1" << endl;
    DEBUG("EventController::next_event()");
    WebservEvent *next_event = event->make_next_event(event, this->event_factory);
    cout << "next_event No.2" << endl;
    E_EpollEvent next_epoll_event = event->get_next_epoll_event();
    cout << "next_event No.3" << endl;

    if (next_epoll_event == EPOLL_READ){
    cout << "next_event No.4" << endl;
        MYINFO("EventController::next is epoll read");

    }else if (next_epoll_event == EPOLL_WRITE){
    cout << "next_event No.5" << endl;
        MYINFO("EventController::next is epoll write");
        this->io_multi_controller->modify(event->fd(), EPOLLOUT);
        this->event_manager->erase_event_waiting_reading(event->fd());
        this->event_manager->add_event_waiting_writing(next_event->fd(), next_event);
    }else{
    cout << "next_event No.6" << endl;
        MYINFO("EventController::next is not epoll");
        if (next_event){
            event_manager->push(next_event);
        }
    }

    cout << "next_event No.7" << endl;
    if (next_epoll_event != EPOLL_CONTINUE){
    cout << "next_event No.8" << endl;
        delete event;
    }
    cout << "next_event No.9" << endl;

}

