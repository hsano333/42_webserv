#include "epoll_controller.hpp"
#include "global.hpp"
#include "webserv_event.hpp"
#include "timeout_exception.hpp"
#include <string.h>
#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;


EpollController::EpollController(
            Epoll epoll,
            SocketRepository *socket_repository,
            SocketController *socket_controller,
            FDManager *fd_manager
        ):
        epoll(epoll),
        socket_repository(socket_repository),
        socket_controller(socket_controller),
        fd_manager(fd_manager)
{
    ;
}



EpollController::~EpollController()
{
    ;
}

void EpollController::init_epoll()
{
    MYINFO("EpollController::init_epoll()");
    this->epoll.init();

    std::map<FileDiscriptor, Socket>::const_iterator ite = socket_repository->begin();
    std::map<FileDiscriptor, Socket>::const_iterator end = socket_repository->end();
    while(ite != end){
        FileDiscriptor fd = ite->second.get_socket_fd();
        this->add(fd, EPOLLIN);
        this->fd_manager->add_socket(fd);
        ite++;
    }
}

void EpollController::add(FileDiscriptor const &fd_obj, uint32_t event)
{
    MYINFO("EpollController::add() fd:" + Utility::to_string(fd_obj.to_int())) ;

    int fd = fd_obj.to_int();
    t_epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (epoll_ctl(this->epoll.fd().to_int(), EPOLL_CTL_ADD, fd, &ev) != 0) {
        WARNING("Epoll add Error fd=" + fd_obj.to_string());
        throw std::runtime_error("Epoll add Error");
    }
}

void EpollController::modify(FileDiscriptor const &fd_obj, uint32_t event)
{
    MYINFO("EpollController::modify() fd:" + Utility::to_string(fd_obj.to_int()) + ", event:" + Utility::to_string(event));

    int fd = fd_obj.to_int();
    t_epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (epoll_ctl(this->epoll.fd().to_int(), EPOLL_CTL_MOD, fd, &ev) != 0) {
        WARNING("Epoll modify Error");
    }
}

void EpollController::erase(FileDiscriptor const &fd_obj)
{
    MYINFO("EpollController::erase() fd:" + Utility::to_string(fd_obj.to_int()));
    int fd = fd_obj.to_int();
    t_epoll_event ev;
    ev.data.fd = fd;

    if (epoll_ctl(this->epoll.fd().to_int(), EPOLL_CTL_DEL, fd, &ev) != 0) {
        ERROR("Epoll erase Error");
        throw std::runtime_error("Epoll erase Error");
    }
}

FileDiscriptor const &EpollController::fd()
{
    return (this->epoll.fd());
}

void EpollController::wait(int time_sec)
{
    MYINFO("EpollController::wait() epoll.fd():" + Utility::to_string(epoll.fd()));

    int nfds = epoll_wait(epoll.fd().to_int(), epoll.event_return(), EVENT_MAX_NUMBER, time_sec * 1000);
    MYINFO("epoll nfds=" + Utility::to_string(nfds));
    if(nfds < 0){
        ERROR("EPOLL WAIT ERROR");
        throw std::runtime_error("EPOLL WAIT ERROR");
    }
    epoll.save_executable_events_number(nfds);

}

int EpollController::executable_event_number()
{
    return (epoll.executable_event_number());
}

t_epoll_event *EpollController::event_return_wrapper()
{
    return (this->epoll.event_return());
}

