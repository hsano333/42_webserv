#include "epoll_controller.hpp"
#include "global.hpp"
#include "webserv_event.hpp"
#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;


EpollController::EpollController(
            Epoll epoll,
            SocketRepository *socket_repository
        ):
        epoll(epoll),
        socket_repository(socket_repository)
{
    ;
}



EpollController::~EpollController()
{
    ;
}

void EpollController::init_epoll()
{
    //10 is ignored; reference man;
    //this->epfd_ = epoll_create(10);
    this->epoll.init();

    std::map<FileDiscriptor, Socket>::const_iterator ite = socket_repository->begin();
    std::map<FileDiscriptor, Socket>::const_iterator end = socket_repository->end();
    while(ite != end){
        FileDiscriptor fd = ite->second.get_socket_fd();
        this->add(fd, EPOLLIN);
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
}

void EpollController::modify(FileDiscriptor fd_obj, uint32_t event)
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
    int fd = fd_obj.to_int();
    t_epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (epoll_ctl(this->epoll.fd().to_int(), EPOLL_CTL_MOD, fd, &ev) != 0) {
        ERROR("Epoll modify Error");
        throw std::runtime_error("Epoll modify Error");
        //return (false);
    }
    //return (true);
}

void EpollController::erase(FileDiscriptor fd_obj)
{
    //if (check_error() == false){
        //return (false);
    //}
    int fd = fd_obj.to_int();
    t_epoll_event const *event = this->epoll.event_related_with_fd(fd_obj);
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

FileDiscriptor EpollController::get_fd()
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

    DEBUG("EpollController::wait()");
    MYINFO("EpollController::epoll.fd():" + Utility::to_string(epoll.fd()) + ",epoll.event_size():" + Utility::to_string(epoll.allocated_event_size()));
    int time_msec = -1;
    //if (get_fd_num() > 0) {
        time_msec = 5;
    //}

    //std::vector<t_epoll_event> events_vec;
    int nfds = epoll_wait(epoll.fd().to_int(), epoll.allocated_event_pointer(), epoll.allocated_event_size(), time_msec * 1000);
    //int nfds = epoll_wait(epoll.fd(), epoll.allocated_event_pointer(), 3, time_msec * 1000);
    if(nfds == 0){
        MYINFO("timeout:" + Utility::to_string(time_msec) + "sec");
        std::cout << "time out" << std::endl;
        //timeout todo 
        // add event_manager timeaut value;
    }else{
        //std::cout << " not time out" << std::endl;

    }
    epoll.save_executable_events_number(nfds);
    //int nfds = epoll_wait(this->epfd_, (&(this->events[0])), get_fd_num(), time_msec * 1000);
    //return (nfds);
    //if (this->count_fd() > MAX_FD){
        //this->close_all_fd();
    //}
}

bool EpollController::have_executable_events()
{
    if(epoll.allocated_event_size() > 0){
        return (true);
    }
    return (false);
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
