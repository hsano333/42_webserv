#ifndef EVENT_MANNGER_HPP
#define EVENT_MANNGER_HPP

#include "webserv_event.hpp"
#include "webserv_cleaner.hpp"
#include "MutantStack.hpp"
#include <stack>
#include <queue>
#include <set>

class WebservEvent;
class WebservCleaner;
class EventManager
{
    public:
        EventManager();
        ~EventManager();
        void push(WebservEvent *event);
        void pop();
        WebservEvent *pop_first();
        size_t event_size();
        size_t keep_alive_event_size();
        void add_event_waiting_epoll(FileDiscriptor const &fd, WebservEvent* event);
        void erase_event_waiting_epoll(FileDiscriptor const &fd);
        WebservEvent* pop_event_waiting_epoll(FileDiscriptor &fd);
        bool check_timeout();
        void close_all_events_waiting_epoll(WebservCleaner *cleaner);
        void close_all_events();
        void retrieve_clean_events(std::set<WebservEvent *> &event_return);
        bool find(FileDiscriptor &fd);

        void add_events_will_deleted(FileDiscriptor const &fd, WebservEvent* event);
        void erase_events_will_deleted(FileDiscriptor const &fd);

        //void add_child_events_will_deleted(FileDiscriptor const &fd, WebservEvent* event);
        //void erase_child_events_will_deleted(FileDiscriptor const &fd);
    private:
        // simple deque with iterator
        MutantStack<WebservEvent*, std::deque<WebservEvent*>, std::deque<WebservEvent*> > events;
        std::map<FileDiscriptor, WebservEvent*> events_waiting_epoll;
        std::map<FileDiscriptor, std::set<WebservEvent*> > events_will_deleted;

        //std::map<FileDiscriptor, std::set<void*> > child_events_will_deleted;
};

#endif
