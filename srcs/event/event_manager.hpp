#ifndef EVENT_MANNGER_HPP
#define EVENT_MANNGER_HPP

#include "webserv_event.hpp"
#include "MutantStack.hpp"
#include <stack>
#include <queue>

class EventManager
{
    public:
        EventManager();
        ~EventManager();
        //void add(WebservEvent *event);
        void push(WebservEvent *event);
        void pop();
        WebservEvent *pop_first();
        size_t event_size();
        void increase_timeout_count(int count);
    private:
        //MutantStack<WebservEvent *> events;
        MutantStack<WebservEvent*, std::deque<WebservEvent*>, std::queue<WebservEvent*> > events;
        //std::queue<WebservEvent *> events;


};

#endif
