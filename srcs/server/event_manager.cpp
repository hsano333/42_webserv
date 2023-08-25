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

WebservEvent *EventManager::top()
{
    return (this->events.top());
}

size_t EventManager::event_size()
{
    return (this->events.size());

}



