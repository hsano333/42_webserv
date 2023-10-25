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
    //10 is ignored; reference man;
    //this->epfd_ = epoll_create(10);
    this->epoll.init();

    std::map<FileDiscriptor, Socket>::const_iterator ite = socket_repository->begin();
    std::map<FileDiscriptor, Socket>::const_iterator end = socket_repository->end();
    while(ite != end){
    MYINFO("EpollController::init_epoll()2");
        FileDiscriptor fd = ite->second.get_socket_fd();
        std::cout << "init_epoll() fd:" << fd << std::endl;
        this->add(fd, EPOLLIN);
        this->fd_manager->add_socket(fd);
        ite++;
    }
}

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
t_epoll_event* EpollController::get_event(FileDiscriptor fd)
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

void EpollController::add(FileDiscriptor fd_obj, uint32_t event)
{
    MYINFO("EpollController::add() fd:" + Utility::to_string(fd_obj.to_int()) + ", epfd:" + Utility::to_string(this->epoll.fd().to_int()));
    //if (check_error() == false){
        //return (false);
    //}

    int fd = fd_obj.to_int();
    t_epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (epoll_ctl(this->epoll.fd().to_int(), EPOLL_CTL_ADD, fd, &ev) != 0) {
        ERROR("Epoll add Error");
        throw std::runtime_error("Epoll add Error");
    }
    this->epoll.expand_allocated_space();
    //fd_map.insert(fd, true);
}

void EpollController::modify(FileDiscriptor fd_obj, uint32_t event)
{
    MYINFO("EpollController::modify() fd:" + Utility::to_string(fd_obj.to_int()) + ", event:" + Utility::to_string(event));

    int fd = fd_obj.to_int();
    t_epoll_event *ev = this->epoll.event_from_fd(fd);
    ev->events = event;
    //ev.data.fd = fd;
    if (epoll_ctl(this->epoll.fd().to_int(), EPOLL_CTL_MOD, fd, ev) != 0) {
        ERROR("Epoll modify Error");
        throw std::runtime_error("Epoll modify Error");
    }
}

void EpollController::erase(FileDiscriptor fd_obj)
{
    MYINFO("EpollController::erase() fd:" + Utility::to_string(fd_obj.to_int()));
    //if (check_error() == false){
        //return (false);
    //}
    int fd = fd_obj.to_int();
    //t_epoll_event const *event = this->epoll.event_related_with_fd(fd_obj);
    t_epoll_event ev;
    ev.data.fd = fd;
    //ev.events = event->events;
    //if (ite_ev == events.end()){
        //return (true);
    //}

    //std::vector<t_epoll_event*>::iterator ite_ev = events.find(fd);
    //if(ite_ev == events.end()){
        //return (true);
    //}
    //t_epoll_event* ev = (*ite_ev);
    if (epoll_ctl(this->epoll.fd().to_int(), EPOLL_CTL_DEL, fd, &ev) != 0) {
        ERROR("Epoll erase Error");
        throw std::runtime_error("Epoll erase Error");
        //return (false);
    }
    //std::vector<t_epoll_event>::iterator ite_ev = std::find(events.begin(), events.end(), ev);
    //delete (ev);
    //this->events.erase(ite_ev);
    //return (true);
    this->epoll.contract_allocated_space();

}

FileDiscriptor EpollController::fd()
{
    return (this->epoll.fd());
}

/*
size_t EpollController::get_fd_num()
{
    return (events.size());
}
*/

void EpollController::wait()
{
    MYINFO("EpollController::wait() epoll.fd():" + Utility::to_string(epoll.fd()));
    int time_msec = -1;
    //if (get_fd_num() > 0) {
        time_msec = 5;
    //}

    //std::vector<t_epoll_event> events_vec;
    int nfds = epoll_wait(epoll.fd().to_int(), epoll.allocated_event_pointer(), epoll.allocated_event_size(), time_msec * 1000);
    if(nfds < 0){
        ERROR("EPOLL WAIT ERROR");
        throw std::runtime_error("EPOLL WAIT ERROR");
    }
    epoll.save_executable_events_number(nfds);
    if(nfds == 0){
        MYINFO("timeout:" + Utility::to_string(time_msec) + "sec");
        std::string tmp = Utility::to_string(time_msec);
        throw TimeoutException(tmp);
    }else{
        t_epoll_event *ev = epoll.allocated_event_pointer();
        for(int i=0;i<nfds;i++){
            MYINFO("wait end fd:" + Utility::to_string(ev[i].data.fd) );
        }
    }
}

int EpollController::executable_event_number()
{
    return (epoll.executable_event_number());
    //if(epoll.allocated_event_size() > 0){
        //return (true);
    //}
    //return (false);
}

std::vector<t_epoll_event> & EpollController::take_out_event()
{
    /*
    std::vector<t_epoll_event> io_events = epoll.get_events();
    size_t io_events_size = epoll.allocated_event_size();
    for(size_t i=0;i<io_events_size;i++){
        WebservEvent *event = WebservEvent::from_epoll_event((io_events[i]));
        (void)event;
        //events[i];
    }
    return (io_events);
    */
    epoll.save_executable_events_number(0);
    return (epoll.get_events());
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
