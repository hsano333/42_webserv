#include "event_manager.hpp"
#include "webserv_keep_alive_event.hpp"



EventManager::EventManager()
{
    ;
}

EventManager::~EventManager()
{
    ;
}

void EventManager::push(WebservEvent *event)
{
    DEBUG("EventManager::push()");
    this->events.push(event);
}

void EventManager::pop()
{
    DEBUG("EventManager::pop()");
    this->events.pop();
}

WebservEvent *EventManager::pop_first()
{
    DEBUG("EventManager::pop_first()");
    WebservEvent *top = this->events.top();
    if(this->events.size() > 0){
        this->events.pop();
    }
    return (top);
}

size_t EventManager::event_size()
{
    return (this->events.size());
}

size_t EventManager::keep_alive_event_size()
{
    size_t cnt = 0;
    MutantStack<WebservEvent*>::iterator ite = this->events.begin();
    MutantStack<WebservEvent*>::iterator end = this->events.end();
    while(ite != end){
        WebservEvent *event = *ite;
        if(event->is_keepalive()){
            cnt++;
        }
        ite++;
    }
    return (cnt);
}

bool EventManager::find(FileDiscriptor &fd)
{

    MutantStack<WebservEvent *>::iterator ite;
    MutantStack<WebservEvent *>::iterator end;
    while(ite != end){
        WebservEvent *event = *ite;
        if(fd == event->entity()->fd()){
            return (true);
        }
        ite++;
    }
    return (false);
}

void EventManager::add_event_waiting_epoll(FileDiscriptor const &fd, WebservEvent* event)
{
    DEBUG("add_event_waiting_epoll() fd:" + fd.to_string());
    this->events_waiting_epoll.insert(std::make_pair(fd, event));
}

void EventManager::erase_event_waiting_epoll(FileDiscriptor const &fd)
{
    DEBUG("erase_event_waiting_epoll() fd:" + fd.to_string());
    this->events_waiting_epoll.erase(fd);
}

WebservEvent* EventManager::pop_event_waiting_epoll(FileDiscriptor &fd)
{
    DEBUG("EventManager::pop_event_waiting_epoll fd:" + Utility::to_string(fd.to_int()));
    if(this->events_waiting_epoll.find(fd) == this->events_waiting_epoll.end()){
        return (NULL);
    }
    WebservEvent *event = (this->events_waiting_epoll[fd]);
    this->events_waiting_epoll.erase(fd);
    return (event);
}

void EventManager::count_up_to_all_event(int time)
{
    {
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_epoll.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_epoll.end();

        while(ite != end){
            ite->second->increase_timeout_count(time);
            ite++;
        }
    }

    {
        MutantStack<WebservEvent *>::iterator ite;
        MutantStack<WebservEvent *>::iterator end;
        while(ite != end){
            (*ite)->increase_timeout_count(time);
            ite++;
        }
    }
}

bool EventManager::check_timeout()
{
    {
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_epoll.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_epoll.end();
        while(ite != end){
            if(ite->second->timeout_count() >= TIMEOUT){
                return (true);
            }
            ite++;
        }
    }
    {
        MutantStack<WebservEvent *>::iterator ite;
        MutantStack<WebservEvent *>::iterator end;
        while(ite != end){
            if((*ite)->timeout_count() >= TIMEOUT){
                return (true);
            }
            ite++;
        }
    }
    return (false);
}

void EventManager::retrieve_timeout_events(std::vector<WebservEvent *> &event_return)
{
    DEBUG("retrieve_timeout_events()");
    {
        std::vector<FileDiscriptor> tmp_fds;
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_epoll.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_epoll.end();
        while(ite != end){
            if(ite->second->timeout_count() >= TIMEOUT){
                tmp_fds.push_back(ite->first);
            }
            ite++;
        }
        for(size_t i=0;i<tmp_fds.size();i++){
            WebservEvent *event = this->pop_event_waiting_epoll(tmp_fds[i]);
            if(event != NULL){
                event_return.push_back(event);
            }
        }

    }
    {

        std::vector<WebservEvent *> event_saved;
        while(events.size() > 0)
        {
            WebservEvent *event = this->pop_first();
            if(event->timeout_count() >= TIMEOUT){
                event_return.push_back(event);
            }else{
                event_saved.push_back(event);
            }
        }
        for(int i=event_saved.size()-1;i>=0;i--){
            this->events.push(event_saved[i]);
        }
    }
}

void EventManager::close_all_events_waiting_epoll(WebservCleaner *cleaner)
{
    DEBUG("EventManager::close_all_events_waiting_epoll");
    std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_epoll.begin();
    std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_epoll.end();
    std::vector<WebservEvent*> tmp;
    while(ite != end){
        tmp.push_back(ite->second);
        ite++;
    }
    DEBUG("EventManager::close_all_events_waiting_epoll size:" + Utility::to_string(tmp.size()));
    for(size_t i=0;i<tmp.size();i++){
        cleaner->clean(tmp[i]->entity(), true);
        delete tmp[i];
    }
}

void EventManager::close_all_events()
{
    DEBUG("EventManager::close_all_events()");
    while(events.size() > 0){
        WebservEvent *event = this->pop_first();
        close(event->entity()->fd().to_int());
        delete event->entity();
        delete event;
    }
}

