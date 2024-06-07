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
    std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_epoll.find(fd);
    if(ite == this->events_waiting_epoll.end()){
        this->events_waiting_epoll.insert(std::make_pair(fd, event));
    }else{
        this->events_waiting_epoll[fd] = event;
    }

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
            if(ite->second && ite->second->check_timeout(now)){
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
            if(*ite && (*ite)->check_timeout(now)){
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
            if(ite->second == NULL){
                ite++;
                continue;
            }
            if(ite->second->check_timeout(now)){
                if(ite->second->entity()->event_error() == Timeout){
                    ite->second->entity()->set_event_error(Timeout2);
                }else{
                    ite->second->entity()->set_event_error(Timeout);
                }
                timeout_fds.push_back(ite->first);
                ite++;
                continue;
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
        delete tmp[i];
    }
}

void EventManager::close_all_events()
{
    DEBUG("EventManager::close_all_events() size:" + Utility::to_string(events.size()));
    while(events.size() > 0){
        WebservEvent *event = this->pop_first();
        close(event->entity()->fd().to_int());
        delete event->entity();
        delete event;
    }
}

void EventManager::add_events_will_deleted(FileDiscriptor const &fd, WebservEvent* event)
{
    DEBUG("add_events_will_deleted fd:" + fd.to_string());

    std::map<FileDiscriptor, std::set<WebservEvent*> >::iterator ite = this->events_will_deleted.find(fd);
    if(ite == this->events_will_deleted.end()){
        std::set<WebservEvent*> new_set;
        new_set.insert(event);
        this->events_will_deleted.insert(std::make_pair(fd, new_set));
    }else{
        ite->second.insert(event);
    }

}

WebservEntity *EventManager::erase_events_will_deleted_event(FileDiscriptor const &fd)
{
    WebservEntity *entity = NULL;
    DEBUG("erase_events_will_deleted");
    std::map<FileDiscriptor, std::set<WebservEvent*> >::iterator ite = this->events_will_deleted.find(fd);
    if(ite != this->events_will_deleted.end()){
        std::set<WebservEvent*>::iterator ite_event = ite->second.begin();
        std::set<WebservEvent*>::iterator end_event = ite->second.end();
        while(ite_event != end_event){
            DEBUG("delete erase_events_will_deleted address:" + Utility::to_string(*ite_event));
            entity = (*ite_event)->entity();
            if(entity->io().get_write_fd().to_int() > 0){
                DEBUG("close cgi pipe:" + entity->io().get_write_fd().to_string());
                entity->io().get_write_fd().close();
            }
            if(entity->io().get_read_fd().to_int() > 0){
                DEBUG("close cgi pipe:" + entity->io().get_write_fd().to_string());
                entity->io().get_read_fd().close();
            }
            DEBUG("delete entity address:" + Utility::to_string(entity));
            delete *ite_event;
            ite_event++;
        }
        this->events_will_deleted.erase(ite);
    }
    delete entity;
    return entity;
}

WebservEntity *EventManager::erase_events_will_deleted_except_keepout(FileDiscriptor const &fd, std::vector<FileDiscriptor> *delete_fd)
{
    WebservEntity *entity = NULL;

    std::map<WebservEvent*, FileDiscriptor> tmp_map;
    std::map<FileDiscriptor, WebservEvent*>::iterator wait_ite = this->events_waiting_epoll.begin();
    std::map<FileDiscriptor, WebservEvent*>::iterator wait_end = this->events_waiting_epoll.end();
    while(wait_ite != wait_end){
        tmp_map.insert(std::make_pair(wait_ite->second, wait_ite->first));
        wait_ite++;
    }

    DEBUG("erase_events_will_deleted_except_keepout");
    std::map<FileDiscriptor, std::set<WebservEvent*> >::iterator ite = this->events_will_deleted.find(fd);
    if(ite != this->events_will_deleted.end()){
        std::set<WebservEvent*> save_set;
        std::set<WebservEvent*>::iterator ite_event = ite->second.begin();
        std::set<WebservEvent*>::iterator end_event = ite->second.end();
        while(ite_event != end_event){
            WebservEvent *event = *ite_event;
            entity = event->entity();
            DEBUG("delete erase_events_will_deleted address:" + Utility::to_string(*ite_event));
            if(event->which() != KEEP_ALIVE_EVENT){
                if(tmp_map.find(event) != tmp_map.end()){
                    FileDiscriptor const &tmp_fd = tmp_map[event];
                    this->events_waiting_epoll.erase(tmp_fd);
                        delete_fd->push_back(fd);
                }
                delete event;
            }else{
                DEBUG("not delete erase_events_will_deleted address:" + Utility::to_string(*ite_event));
                save_set.insert(event);
            }
            ite_event++;
        }
        this->events_will_deleted[fd] = save_set;
    }
    return entity;
}

