#ifndef WEBSERV_TIME_KEEPER
#define WEBSERV_TIME_KEEPER
#include "event_manager.hpp"

class WebservTimeKeeper
{
    public:
        WebservTimeKeeper(
                        EventManager *event_manager
                );
        ~WebservTimeKeeper();
        void count_up(int time);
        void timeout(int time);
    private:
        EventManager *event_manager;

};

#endif
