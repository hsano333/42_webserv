#include "event_controller.hpp"

EventController::EventController(EventManager *event_manager) 
    :
        event_manager(event_manager)
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
