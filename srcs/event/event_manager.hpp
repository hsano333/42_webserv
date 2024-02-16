#ifndef EVENT_MANNGER_HPP
#define EVENT_MANNGER_HPP

#include "webserv_event.hpp"
#include "webserv_cleaner.hpp"
#include "MutantStack.hpp"
#include <stack>
#include <queue>

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
        void count_up_to_all_event(int time);
        bool check_timeout();
        void close_all_events_waiting_epoll(WebservCleaner *cleaner);
        void close_all_events();
        void retrieve_timeout_events(std::vector<WebservEvent *> &event_return);
        bool find(FileDiscriptor &fd);
    private:
        MutantStack<WebservEvent*> events;
        MutantStack<WebservEvent*> instant_events;
        std::map<FileDiscriptor, WebservEvent*> events_waiting_epoll;
};

#endif
