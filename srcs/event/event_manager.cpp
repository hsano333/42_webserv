#include "event_manager.hpp"



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
    //MutantStack<WebservEvent *>::iterator ite;
    //MutantStack<WebservEvent *>::iterator end;
    while(ite != end){
        WebservEvent *event = *ite;
        if(event->which() == KEEPA_ALIVE_EVENT){
            cnt++;
        }
        ite++;
    }
    return (cnt);
}

bool EventManager::find(FileDiscriptor fd)
{

    MutantStack<WebservEvent *>::iterator ite;
    MutantStack<WebservEvent *>::iterator end;
    while(ite != end){
        WebservEvent *event = *ite;
        if(fd == event->fd()){
            return (true);
        }
        ite++;
    }
    return (false);
}

/*
void EventManager::increase_timeout_count(int count)
{
    DEBUG("EventManager::increase_timeout_count()");
    MutantStack<WebservEvent *>::iterator ite;
    MutantStack<WebservEvent *>::iterator end;

    while(ite != end){
        (*ite)->increase_timeout_count(count);
        ite++;
    }
}
*/


void EventManager::add_event_waiting_reading(FileDiscriptor fd, WebservEvent* event)
{
    if (this->events_waiting_reading.find(fd) != this->events_waiting_reading.end())
    {
        event->increase_timeout_count(1);
    }
    DEBUG("add_event_waiting_reading() fd:" + fd.to_string());
    this->events_waiting_reading.insert(std::make_pair(fd, event));
}

void EventManager::erase_event_waiting_reading(FileDiscriptor fd)
{
    DEBUG("erase_event_waiting_reading() fd:" + fd.to_string());
    this->events_waiting_reading.erase(fd);
}

WebservEvent* EventManager::get_event_waiting_reading(FileDiscriptor fd)
{
    if(this->events_waiting_reading.find(fd) == this->events_waiting_reading.end()){
        return (NULL);
    }
    return (this->events_waiting_reading[fd]);
}


void EventManager::add_event_waiting_writing(FileDiscriptor fd, WebservEvent* event)
{
    if (this->events_waiting_writing.find(fd) != this->events_waiting_writing.end())
    {
        if (event != this->events_waiting_writing[fd]){
            delete this->events_waiting_writing[fd];
        }
    }else{
        event->increase_timeout_count(1);
    }
    this->events_waiting_writing.insert(std::make_pair(fd, event));
}

void EventManager::erase_event_waiting_writing(FileDiscriptor fd)
{
    DEBUG("erase_event_waiting_writing() fd:" + fd.to_string());
    this->events_waiting_writing.erase(fd);
}

WebservEvent* EventManager::get_event_waiting_writing(FileDiscriptor fd)
{
    if(this->events_waiting_writing.find(fd) == this->events_waiting_writing.end()){
        return (NULL);
    }
    return (this->events_waiting_writing[fd]);
}




void EventManager::count_up_to_all_event(int time)
{
    {
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_reading.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_reading.end();

        while(ite != end){
            ite->second->increase_timeout_count(time);
            ite++;
        }
    }
    {
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_writing.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_writing.end();
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
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_reading.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_reading.end();
        while(ite != end){
            if(ite->second->timeout_count() >= TIMEOUT){
                return (true);
            }
            ite++;
        }
    }
    {
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_writing.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_writing.end();
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
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_reading.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_reading.end();
        while(ite != end){
            if(ite->second->timeout_count() >= TIMEOUT){
                event_return.push_back(ite->second);
            }
            ite++;
        }
        for(size_t i=0;i<event_return.size();i++){
            this->events_waiting_reading.erase(event_return[i]->fd());
        }
    }
    {
        std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_writing.begin();
        std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_writing.end();
        while(ite != end){
            if(ite->second->timeout_count() >= TIMEOUT){
                event_return.push_back(ite->second);
            }
            ite++;
        }
        for(size_t i=0;i<event_return.size();i++){
            this->events_waiting_writing.erase(event_return[i]->fd());
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

void EventManager::close_all_events_waiting_reading(WebservCleaner &cleaner)
{
    DEBUG("EventManager::close_all_events_waiting_reading");
    std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_reading.begin();
    std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_reading.end();
    std::vector<WebservEvent*> tmp;
    while(ite != end){
        tmp.push_back(ite->second);
        ite++;
    }
    for(size_t i=0;i<tmp.size();i++){
        cleaner.clean(tmp[i], true);
    }
}

void EventManager::close_all_events_waiting_writing(WebservCleaner &cleaner)
{
    DEBUG("EventManager::close_all_events_waiting_writing");
    std::map<FileDiscriptor, WebservEvent*>::iterator ite = this->events_waiting_writing.begin();
    std::map<FileDiscriptor, WebservEvent*>::iterator end = this->events_waiting_writing.end();
    std::vector<WebservEvent*> tmp;
    while(ite != end){
        tmp.push_back(ite->second);
        ite++;
    }
    DEBUG("EventManager:: tmp.size():" + Utility::to_string(tmp.size()));
    for(size_t i=0;i<tmp.size();i++){
        DEBUG("EventManager:: which):" + Utility::to_string(tmp[i]->which()));
        cleaner.clean(tmp[i], true);
    }
}

void EventManager::close_all_events()
{
    DEBUG("EventManager::close_all_events()");
    while(events.size() > 0){
        WebservEvent *event = this->pop_first();
        close(event->fd().to_int());
        delete event->req();
        delete event;
    }
}




/*
void EventManager::delete_all_waiting_writing()
{
    return (this->events_waiting_writing[fd]);
}
*/

