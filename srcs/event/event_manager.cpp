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
    WebservEvent *first = this->events.front();
    if(this->events.size() > 0){
        this->events.pop();
    }
    return (first);
}

size_t EventManager::event_size()
{
    return (this->events.size());

}

void EventManager::increase_timeout_count(int count)
{
    DEBUG("EventManager::increase_timeout_count()");
    MutantStack<WebservEvent *>::iterator ite;
    MutantStack<WebservEvent *>::iterator end;

    while(ite != end){
        (*ite)->increase_and_check_timeout_count(count);
        ite++;
    }
}

void EventManager::add_event_waiting_writing(FileDiscriptor fd, WebservEvent* event)
{
    this->events_waiting_writing.insert(std::make_pair(fd, event));
}

void EventManager::erase_event_waiting_writing(FileDiscriptor fd)
{
    this->events_waiting_writing.erase(fd);
}

WebservEvent* EventManager::get_event_waiting_writing(FileDiscriptor fd)
{
    if(this->events_waiting_writing.find(fd) == this->events_waiting_writing.end()){
        return (NULL);
    }
    return (this->events_waiting_writing[fd]);
}

