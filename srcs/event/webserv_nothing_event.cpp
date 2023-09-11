
#include "webserv_nothing_event.hpp"
#include "global.hpp"

WebservNothingEvent::WebservNothingEvent()
{
    ;
}


WebservNothingEvent::~WebservNothingEvent()
{
    ;
}

EWebservEvent WebservNothingEvent::which()
{
    return (NOTHING_EVENT);
}

FileDiscriptor WebservNothingEvent::get_fd()
{
    return (FileDiscriptor::from_int(0));
}

Request *WebservNothingEvent::req()
{
    return (NULL);
}

Response *WebservNothingEvent::res()
{
    return (NULL);
}

void WebservNothingEvent::increase_and_check_timeout_count(int count)
{
    (void)(count);
}
