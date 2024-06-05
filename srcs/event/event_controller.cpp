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

void EventController::change_write_event(WebservEvent *event)
{
    DEBUG("EventController::change_write_event() fd=" + event->entity()->fd().to_string());
    (void)event;
    this->io_multi_controller->modify(event->entity()->fd(), EPOLLIN | EPOLLONESHOT);

}

void EventController::set_next_epoll_event(WebservEvent *event, WebservEvent *next_event)
{
    DEBUG("EventController::set_next_epoll_event");
    if(next_event == NULL){
        MYINFO("EventController::next_epoll_event is NULL");
        return;
    }

    E_EpollEvent next_epoll_event = event->epoll_event();

    if (next_epoll_event == EPOLL_READ){
        MYINFO("EventController::next is epoll read fd=" + next_event->entity()->fd().to_string());
        this->io_multi_controller->modify(next_event->entity()->fd(), EPOLLIN | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->fd(), next_event);
    }else if (next_epoll_event == EPOLL_WRITE){
        MYINFO("EventController::next is epoll write fd=" + next_event->entity()->fd().to_string());
        try{
            this->io_multi_controller->modify(next_event->entity()->fd(), EPOLLOUT | EPOLLONESHOT);
        }catch(std::runtime_error &e){
            WARNING("This error is ignored:" + Utility::to_string(e.what()));
        }

        this->event_manager->add_event_waiting_epoll(next_event->entity()->fd(), next_event);
    }else if (next_epoll_event == EPOLL_ADD_WRITE){
        MYINFO("EventController::next is epoll add writing");
        this->io_multi_controller->add(next_event->entity()->fd(), EPOLLOUT | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->fd(), next_event);
    //}else if (next_epoll_event == EPOLL_FOR_CGI_OUT){  // from cgi out to socket
        //MYINFO("EventController::next is epoll EPOLL_FOR_CGI_OUT");
        //FileDiscriptor const &socket_fd = next_event->entity()->socket_fd();

        //this->io_multi_controller->add(next_event->entity()->io().get_write_fd(), EPOLLOUT | EPOLLONESHOT);
        //this->event_manager->add_event_waiting_epoll(next_event->entity()->io().get_write_fd(), next_event);
        //this->fd_manager->add_socket_and_epoll_fd(next_event->entity()->io().get_write_fd(), socket_fd);
    //}else if (next_epoll_event == EPOLL_NONE_FOR_GET_CGI){  // from cgi out to socket

    }else if (next_epoll_event == EPOLL_READ_FOR_POST_CGI){ // from socket to cgi in
        MYINFO("EventController::next is epoll read for post cgi fd=" + next_event->entity()->fd().to_string());
        this->io_multi_controller->modify(next_event->entity()->fd(), EPOLLIN | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->fd(), next_event);

        // cgi出力検知を一時停止
        this->io_multi_controller->erase(next_event->entity()->io().get_read_fd());

    }else if (next_epoll_event == EPOLL_WRITE_FOR_POST_CGI){ // from socket to cgi in
        MYINFO("EventController::next is epoll write for post cgi fd=" + next_event->entity()->fd().to_string());
        this->io_multi_controller->modify(next_event->entity()->fd(), EPOLLOUT | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->fd(), next_event);

        // cgi入力検知を一時停止
        this->io_multi_controller->erase(next_event->entity()->io().get_write_fd());

    }else if (next_epoll_event == EPOLL_FOR_CGI_WAIT_CGI){ // from socket to cgi in
        MYINFO("EventController::next is epoll EPOLL_FOR_CGI_WAIT_CGI write_fd:" + Utility::to_string(next_event->entity()->io().get_write_fd()));
        FileDiscriptor const &socket_fd = next_event->entity()->socket_fd();

        // CGI OUT
        try{
            this->io_multi_controller->add(next_event->entity()->io().get_write_fd(), EPOLLIN | EPOLLONESHOT);
        }catch(std::runtime_error &e){
            WARNING("This error is ignored:" + Utility::to_string(e.what()));
        }
        this->event_manager->add_event_waiting_epoll(next_event->entity()->io().get_write_fd(), next_event);
        this->fd_manager->add_socket_and_epoll_fd(next_event->entity()->io().get_write_fd(), socket_fd);
        /*
    }else if (next_epoll_event == EPOLL_FOR_CGI_IN){ // from socket to cgi in
        MYINFO("EventController::next is epoll EPOLL_FOR_CGI_IN");
        FileDiscriptor const &socket_fd = next_event->entity()->socket_fd();

        this->io_multi_controller->add(next_event->entity()->io().get_read_fd(), EPOLLIN | EPOLLONESHOT);
        this->event_manager->add_event_waiting_epoll(next_event->entity()->io().get_read_fd(), next_event);
        this->fd_manager->add_socket_and_epoll_fd(next_event->entity()->io().get_read_fd(), socket_fd);
        */

    }else if (next_epoll_event == EPOLL_FOR_CGI_POST){
        MYINFO("EventController::next is epoll EPOLL_FOR_CGI_POST read_fd:" + Utility::to_string(next_event->entity()->io().get_read_fd()));
        MYINFO("EventController::next is epoll EPOLL_FOR_CGI_POST write_fd:" + Utility::to_string(next_event->entity()->io().get_write_fd()));
        FileDiscriptor const &socket_fd = next_event->entity()->socket_fd();

        try{
            // CGI IN
            this->io_multi_controller->add(next_event->entity()->io().get_write_fd(), EPOLLIN | EPOLLONESHOT);
        }catch(std::runtime_error &e){
            WARNING("This error is ignored:" + Utility::to_string(e.what()));
        }
        this->event_manager->add_event_waiting_epoll(next_event->entity()->io().get_write_fd(), next_event);
        this->fd_manager->add_socket_and_epoll_fd(next_event->entity()->io().get_write_fd(), socket_fd);

            // CGI OUT
        try{
            this->io_multi_controller->add(next_event->entity()->io().get_read_fd(), EPOLLOUT | EPOLLONESHOT);
        }catch(std::runtime_error &e){
            WARNING("This error is ignored:" + Utility::to_string(e.what()));
        }
        this->event_manager->add_event_waiting_epoll(next_event->entity()->io().get_read_fd(), next_event);
        this->fd_manager->add_socket_and_epoll_fd(next_event->entity()->io().get_read_fd(), socket_fd);

    }else if(next_event){
        MYINFO("EventController::next is not epoll");
        this->event_manager->push(next_event);
    }
}

void EventController::erase_epoll_event(FileDiscriptor const &fd)
{
    if(fd.to_int() <= 0){
        return;
    }
    DEBUG("EventController::erase_epoll_event fd:" + fd.to_string());
        this->event_manager->erase_event_waiting_epoll(fd);
    try{
        //this->io_multi_controller->erase(fd);
    }catch(std::runtime_error &e){
        WARNING("[EventController::erase_epoll_event] This error is ignored:" + Utility::to_string(e.what()));
    }
    //fd.close();
}



