#ifndef EVENT_MANNGER_HPP
#define EVENT_MANNGER_HPP

#include "webserv_event.hpp"
#include <stack>

class EventManager
{
    public:
        EventManager();
        ~EventManager();
        //void add(WebservEvent *event);
        void push(WebservEvent *event);
        void pop();
        WebservEvent *top();
        size_t event_size();
    private:
        std::stack<WebservEvent *> events;


};

#endif
