#include "epoll_controller.hpp"
#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;

EpollController::EpollController(
            Epoll epoll,
            SocketManager *socket_manager
        ):
        epoll(epoll),
        socket_manager(socket_manager)
{
    ;
}

EpollController::~EpollController()
{
    ;
}

/*
void EpollController::init()
{
    //10 is ignored; reference man;
    this->epfd_ = epoll_create(10);
}
*/

/*
EpollController* EpollController::_singleton = NULL;
EpollController::~EpollController()
{
    delete (_singleton);
}
*/


/*
int EpollController::epfd()
{
    return (epfd_);
}
*/


/*
bool EpollController::check_error()
{
    if(_error && epfd_ < 0){
        this->epfd_ = epoll_create(1);
    }
    if (this->epfd_ > 0){
        return (true);
    }
    return (false);
}
*/

/*
t_epoll_event* EpollController::get_event(int fd)
{
    std::vector<t_epoll_event*>::iterator ite = events.begin();
    std::vector<t_epoll_event*>::iterator end = events.end();

    for (; ite != end; ite++){
        t_epoll_event* tmp = *ite;
        if(tmp->data.fd == fd){
            return (*ite);
        }
    }
    return (NULL);
}
*/
/*
void EpollController::erase_event(t_epoll_event* ev)
{
    this->events.erase(ev);
}
*/

void EpollController::add(int fd, uint32_t event)
{
    //if (check_error() == false){
        //return (false);
    //}
    t_epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (epoll_ctl(this->epoll.fd(), EPOLL_CTL_ADD, fd, &ev) != 0) {
        ERROR("Epoll add Error");
        throw std::runtime_error("Epoll add Error");
    }
    this->epoll.expand_events();
}

void EpollController::modify(int fd, uint32_t event)
{
    //if (check_error() == false){
        //return (false);
    //}

    //std::vector<epoll_event*>::iterator ite_ev = events.find(fd);
    //if(ev == events.end()){
        //return (false);
    //}
    //std::vector<t_epoll_event>::iterator ite_ev = get_event(fd);
    //if (ite_ev == events.end()){
        //return (false);
    //}
    //t_epoll_event* ev = &(*ite_ev);
    //ev->events = this->epoll.event();
    //t_epoll_event const *ev = this->epoll.event_related_with_fd(fd);
    //cout << "EPOLL CTL  fd:" << fd << endl;
    //ev.data.fd = event;
    t_epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (epoll_ctl(this->epoll.fd(), EPOLL_CTL_MOD, fd, &ev) != 0) {
        ERROR("Epoll modify Error");
        throw std::runtime_error("Epoll modify Error");
        //return (false);
    }
    //return (true);
}

void EpollController::erase(int fd)
{
    //if (check_error() == false){
        //return (false);
    //}
    t_epoll_event const *event = this->epoll.event_related_with_fd(fd);
    t_epoll_event ev;
    ev.data.fd = fd;
    ev.events = event->events;
    //if (ite_ev == events.end()){
        //return (true);
    //}

    //std::vector<t_epoll_event*>::iterator ite_ev = events.find(fd);
    //if(ite_ev == events.end()){
        //return (true);
    //}
    //t_epoll_event* ev = (*ite_ev);
    if (epoll_ctl(this->epoll.fd(), EPOLL_CTL_DEL, fd, &ev) != 0) {
        ERROR("Epoll erase Error");
        throw std::runtime_error("Epoll erase Error");
        //return (false);
    }
    //std::vector<t_epoll_event>::iterator ite_ev = std::find(events.begin(), events.end(), ev);
    //delete (ev);
    //this->events.erase(ite_ev);
    //return (true);
    this->epoll.contract_events();

}

int EpollController::get_fd()
{
    return (0);
}

/*
size_t EpollController::get_fd_num()
{
    return (events.size());
}
*/

void EpollController::wait()
{
    int time_msec = -1;
    //if (get_fd_num() > 0) {
        time_msec = 5;
    //}

    //std::vector<t_epoll_event> events_vec;
    int nfds = epoll_wait(epoll.fd(), epoll.returned_events_pointer(), epoll.event_size(), time_msec * 1000);
    if(nfds == 0){
        //timeout todo 
        // add event_manager timeaut value;

    }
    epoll.set_event_number(nfds);
    //int nfds = epoll_wait(this->epfd_, (&(this->events[0])), get_fd_num(), time_msec * 1000);
    //return (nfds);
    //if (this->count_fd() > MAX_FD){
        //this->close_all_fd();
    //}
}

/*
EpollController* EpollController::get_instance()
{
    if(_singleton == NULL){
        _singleton = new EpollController();
    }
    return _singleton;
}
*/
