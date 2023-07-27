#include "epoll_manager.hpp"
#include <iostream>
#include <string>
#include <algorithm>

using std::cout;
using std::endl;

EpollManager::EpollManager() :  _error(false)
{
    this->_epfd = epoll_create(10);
    if (_epfd < 0) {
        _error = true;
    }
}

EpollManager* EpollManager::_singleton = NULL;
EpollManager::~EpollManager()
{
    delete (_singleton);
}

bool EpollManager::get_error()
{
    return (_error);
}

bool EpollManager::check_error()
{
    if(_error && _epfd < 0){
        this->_epfd = epoll_create(1);
    }
    if (this->_epfd > 0){
        return (true);
    }
    return (false);
}
std::vector<struct epoll_event>::iterator EpollManager::get_event(int fd)
{
    std::vector<struct epoll_event>::iterator ite = _events.begin();
    std::vector<struct epoll_event>::iterator end = _events.end();

    for (; ite != end; ite++){
        //tmp = *ite;
        struct epoll_event& tmp = *ite;
        if(tmp.data.fd == fd){
            return (ite);
        }
    }
    return (_events.end());
}

bool EpollManager::add(int fd, uint32_t event)
{
    if (check_error() == false){
        return (false);
    }
    struct epoll_event ev;
    ev.events = event;
    ev.data.fd = fd;
    if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, fd, &ev) != 0) {
        cout << "epoll add failure" << endl;
        cout << "epoll add failure" << endl;
        cout << "epoll add failure" << endl;
        cout << "epoll add failure" << endl;
        //delete ev;
        return (false);
    }
    this->_events.push_back(ev);
    return (true);
}

bool EpollManager::modify(int fd, uint32_t event)
{
    if (check_error() == false){
        return (false);
    }

    //std::vector<epoll_event*>::iterator ite_ev = _events.find(fd);
    //if(ev == _events.end()){
        //return (false);
    //}
    std::vector<struct epoll_event>::iterator ite_ev = get_event(fd);
    if (ite_ev == _events.end()){
        return (false);
    }
    struct epoll_event* ev = &(*ite_ev);
    ev->events = event;
    cout << "EPOLL CTL  fd:" << fd << endl;
    if (epoll_ctl(this->_epfd, EPOLL_CTL_MOD, fd, ev) != 0) {
        return (false);
    }
    return (true);
}

bool EpollManager::erase(int fd)
{
    if (check_error() == false){
        return (false);
    }
    std::vector<struct epoll_event>::iterator ite_ev = get_event(fd);
    if (ite_ev == _events.end()){
        return (true);
    }

    //std::vector<struct epoll_event*>::iterator ite_ev = _events.find(fd);
    //if(ite_ev == _events.end()){
        //return (true);
    //}
    struct epoll_event* ev = &(*ite_ev);
    if (epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, ev) != 0) {
        return (false);
    }
    //std::vector<struct epoll_event>::iterator ite_ev = std::find(_events.begin(), _events.end(), ev);
    //delete (ev);
    this->_events.erase(ite_ev);
    return (true);
}

size_t EpollManager::get_fd_num()
{
    return (_events.size());
}

int EpollManager::wait(t_epoll_event* event)
{
    int time_msec = -1;
    if (get_fd_num() > 0) {
        time_msec = 5;
    }

    //std::vector<struct epoll_event> events_vec;
    int nfds = epoll_wait(this->_epfd, event, get_fd_num(), time_msec * 1000);
    //int nfds = epoll_wait(this->_epfd, (&(this->_events[0])), get_fd_num(), time_msec * 1000);
    return (nfds);
    //if (this->count_fd() > MAX_FD){
        //this->close_all_fd();
    //}
    /*
    if (nfds == 0) {
        this->timeout(time_msec);
        continue;
    } else if (nfds < 0) {
        cout << "Epoll Wait Error:" << strerror(errno) << endl;
        return;
    }
    */
}

EpollManager* EpollManager::get_instance()
{
    if(_singleton == NULL){
        _singleton = new EpollManager();
    }
    return _singleton;
}
