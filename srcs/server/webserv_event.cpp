#include "webserv_event.hpp"
#include "webserv_event_read.hpp"
#include "global.hpp"
#include "read_socket.hpp"

/*
WebservEvent *WebservEvent::from_epoll_event(t_epoll_event const &event_epoll)
{
    FileDiscriptor fd = FileDiscriptor::from_int(event_epoll.data.fd);
    if(event_epoll.events == EPOLLIN){
        MYINFO("WebservEvent::from_epoll_event: EPOLLIN");
        cout << "fd:" << fd << endl;
        int tmp_fd = this->socket_controller->accept_request(fd);
        WebservReadEvent *event = new WebservReadEvent(fd);
        return (event);
    }else if(event_epoll.events == EPOLLOUT){
        MYINFO("WebservEvent::from_epoll_event: EPOLLOUT");
        WebservReadEvent *event = new WebservReadEvent(fd);
        return (event);
    }else{
        WARNING("Epoll event type is undefined");
        throw std::runtime_error("Epoll event type is undefined");

    }
    return (NULL);
}
*/
