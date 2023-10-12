#include "webserv_time_keeper.hpp"

WebservTimeKeeper::WebservTimeKeeper(
                        EventManager *event_manager
                        ):
                        event_manager(event_manager)
{
    ;
}

WebservTimeKeeper::~WebservTimeKeeper()
{
    ;
}

void WebservTimeKeeper::count_up(int time)
{
    event_manager->count_up_to_all_event(time);
}

void WebservTimeKeeper::timeout(int time)
{
    (void)time;
    //event_manager->count_up_to_all_event(time);
}
