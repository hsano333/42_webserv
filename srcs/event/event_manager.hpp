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


        void add_event_waiting_writing(FileDiscriptor fd, WebservEvent* event);
        void erase_event_waiting_writing(FileDiscriptor fd);
        void take_out_event_waiting_writing(FileDiscriptor fd, WebservEvent* event);
        WebservEvent* get_event_waiting_writing(FileDiscriptor fd);
    private:
        //MutantStack<WebservEvent *> events;
        MutantStack<WebservEvent*, std::deque<WebservEvent*>, std::queue<WebservEvent*> > events;
        std::map<FileDiscriptor, WebservEvent*> events_waiting_writing;
        //std::queue<WebservEvent *> events;


};

#endif
