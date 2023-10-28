#ifndef EVENT_MANNGER_HPP
#define EVENT_MANNGER_HPP

#include "webserv_event.hpp"
#include "webserv_cleaner.hpp"
#include "MutantStack.hpp"
#include <stack>
#include <queue>

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
        //void increase_timeout_count(int count);
        void add_event_waiting_reading(FileDiscriptor fd, WebservEvent* event);
        void erase_event_waiting_reading(FileDiscriptor fd);
        WebservEvent* get_event_waiting_reading(FileDiscriptor fd);
        void add_event_waiting_writing(FileDiscriptor fd, WebservEvent* event);
        void erase_event_waiting_writing(FileDiscriptor fd);
        WebservEvent* get_event_waiting_writing(FileDiscriptor fd);
        void count_up_to_all_event(int time);
        bool check_timeout();
        void close_all_events_waiting_reading(WebservCleaner &cleaner);
        void close_all_events_waiting_writing(WebservCleaner &cleaner);
        void close_all_events();
        void retrieve_timeout_events(std::vector<WebservEvent *> &event_return);
        bool find(FileDiscriptor fd);
    private:
        MutantStack<WebservEvent*> events;
        std::map<FileDiscriptor, WebservEvent*> events_waiting_reading;
        std::map<FileDiscriptor, WebservEvent*> events_waiting_writing;


};

#endif
