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
    this->events.push(event);
}

void EventManager::pop()
{
    this->events.pop();
}

WebservEvent *EventManager::pop_first()
{

    WebservEvent *first = this->events.front();
    this->events.pop();
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


