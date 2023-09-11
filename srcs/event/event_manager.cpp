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
            cout << "test No.2-1" << endl;
    //DEBUG("EventManager::push()");
            cout << "test No.2-2" << endl;
    this->events.push(event);
            cout << "test No.2-2" << endl;
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


