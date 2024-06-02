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
    this->events.push_back(event);
    DEBUG("EventManager::push() event_size:" + Utility::to_string(this->events.size()));
}

void EventManager::pop()
{
    this->events.pop_front();
    DEBUG("EventManager::pop() event_size:" + Utility::to_string(this->events.size()));
}

WebservEvent *EventManager::pop_first()
{
    WebservEvent *top = this->events.front();
    if(this->events.size() > 0){
        this->events.pop_front();
    }
    DEBUG("EventManager::pop_first() event_size:" + Utility::to_string(this->events.size()));
    DEBUG("EventManager::pop_first() which:" + Utility::to_string((top->which())));
    return (top);
}

size_t EventManager::event_size()
{
    DEBUG("EventManager::event_size() :" + Utility::to_string(this->events.size()));
    return (this->events.size());
}

size_t EventManager::keep_alive_event_size()
{
    size_t cnt = 0;
    MutantStack<WebservEvent*>::iterator ite = this->events.begin();
    MutantStack<WebservEvent*>::iterator end = this->events.end();
    while(ite != end){
        WebservEvent *event = *ite;
        if(event->which() == KEEP_ALIVE_EVENT){
            cnt++;
        }
        ite++;
    }
    DEBUG("EventManager::keep_alive_event_size() :" + Utility::to_string(cnt));
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
    DEBUG("EventManager::events_waiting_epoll No.0 size:" + Utility::to_string(events_waiting_epoll.size()));
    this->events_waiting_epoll.insert(std::make_pair(fd, event));
    DEBUG("EventManager::events_waiting_epoll No.1 size:" + Utility::to_string(events_waiting_epoll.size()));
}

void EventManager::erase_event_waiting_epoll(FileDiscriptor const &fd)
{
    DEBUG("erase_event_waiting_epoll() fd:" + fd.to_string());
    this->events_waiting_epoll.erase(fd);
    DEBUG("EventManager::erase_event_waiting_epoll size:" + Utility::to_string(events_waiting_epoll.size()));
}

WebservEvent* EventManager::pop_event_waiting_epoll(FileDiscriptor &fd)
{
    DEBUG("EventManager::pop_event_waiting_epoll fd:" + Utility::to_string(fd.to_int()));
    if(this->events_waiting_epoll.find(fd) == this->events_waiting_epoll.end()){
        return (NULL);
    }
    WebservEvent *event = (this->events_waiting_epoll[fd]);
    this->events_waiting_epoll.erase(fd);
    DEBUG("EventManager::pop_event_waiting_epoll size:" + Utility::to_string(events_waiting_epoll.size()));
    return (event);
}

bool EventManager::check_timeout()
{
    DEBUG("EventManager::check_timeout");
    std::time_t now = std::time(NULL);
    {
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_epoll.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_epoll.end();
        while(ite != end){
            if(ite->second->check_timeout(now)){
                DEBUG("EventManager::check_timeout true epoll");
                return (true);
            }
            ite++;
        }
    }
    {
        MutantStack<WebservEvent *>::iterator ite;
        MutantStack<WebservEvent *>::iterator end;
        while(ite != end){
            if((*ite)->check_timeout(now)){
                DEBUG("EventManager::check_timeout true Event");
                return (true);
            }
            ite++;
        }
    }
    return (false);
}

void EventManager::retrieve_clean_events(std::set<WebservEvent *> &event_return)
{
    std::time_t now = std::time(NULL);
    DEBUG("retrieve_timeout_events()");
    {
        std::vector<FileDiscriptor> timeout_fds;
        std::vector<FileDiscriptor> execve_error_fds;
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_epoll.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_epoll.end();
        DEBUG("epoll wait size:" + Utility::to_string(this->events_waiting_epoll.size()));
        while(ite != end){
            if(ite->second->check_timeout(now)){
                if(ite->second->entity()->event_error() == Timeout){
                    ite->second->entity()->set_event_error(Timeout2);
                }else{
                    ite->second->entity()->set_event_error(Timeout);
                }
                timeout_fds.push_back(ite->first);
            }
            if(ite->second->which() != KEEP_ALIVE_EVENT && ite->second->check_died_child()){
                ite->second->entity()->set_event_error(DiedChild);
                execve_error_fds.push_back(ite->first);
            }
            ite++;
        }
        for(size_t i=0;i<execve_error_fds.size();i++){
            WebservEvent *event = this->pop_event_waiting_epoll(execve_error_fds[i]);
            if(event != NULL){
                event_return.insert(event);
            }
        }
        for(size_t i=0;i<timeout_fds.size();i++){
            WebservEvent *event = this->pop_event_waiting_epoll(timeout_fds[i]);
            if(event != NULL){
                event_return.insert(event);
            }
        }
    }
    {

        std::vector<WebservEvent *> event_saved;
        while(events.size() > 0)
        {
            WebservEvent *event = this->pop_first();

            if(event->check_timeout(now)){
                event->entity()->set_event_error(Timeout);
                event_return.insert(event);
            }else if(event->which() != KEEP_ALIVE_EVENT && event->check_died_child()){
                event->entity()->set_event_error(DiedChild);
                event_return.insert(event);
            }else{
                event_saved.push_back(event);

            }
        }
        for(int i=event_saved.size()-1;i>=0;i--){
            this->events.push_back(event_saved[i]);
        }
    }
}

void EventManager::close_all_events_waiting_epoll(WebservCleaner *cleaner)
{
    (void)cleaner;
    DEBUG("EventManager::close_all_events_waiting_epoll");
    std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_epoll.begin();
    std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_epoll.end();
    std::vector<WebservEvent*> tmp;
    while(ite != end){
        tmp.push_back(ite->second);
        ite++;
    }

    for(size_t i=0;i<tmp.size();i++){
        //if(tmp[i]->which() != KEEP_ALIVE_EVENT){
            //cleaner->clean(tmp[i]->entity(), true);
        //}
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

void EventManager::add_events_will_deleted(FileDiscriptor const &fd, WebservEvent* event)
{

    std::map<FileDiscriptor, std::set<WebservEvent*> >::iterator ite = this->events_will_deleted.find(fd);
    if(ite == this->events_will_deleted.end()){
        std::set<WebservEvent*> new_set;
        new_set.insert(event);
        this->events_will_deleted.insert(std::make_pair(fd, new_set));
    }else{
        ite->second.insert(event);
    }

}

void EventManager::erase_events_will_deleted(FileDiscriptor const &fd)
{
    DEBUG("erase_events_will_deleted");
    std::map<FileDiscriptor, std::set<WebservEvent*> >::iterator ite = this->events_will_deleted.find(fd);
    if(ite != this->events_will_deleted.end()){
        std::set<WebservEvent*>::iterator ite_event = ite->second.begin();
        std::set<WebservEvent*>::iterator end_event = ite->second.end();
        while(ite_event != end_event){
            DEBUG("delete erase_events_will_deleted address:" + Utility::to_string(*ite_event));
            delete *ite_event;
            ite_event++;
        }
        this->events_will_deleted.erase(ite);
    }
}


