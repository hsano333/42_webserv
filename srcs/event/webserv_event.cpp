#include "webserv_event.hpp"
#include "global.hpp"
#include "socket_reader.hpp"

void WebservEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
}

int WebservEvent::timeout_count()
{
    return (this->timeout_count_);
}

/*
bool WebservEvent::completed()
{
    return (this->completed_);
}

void WebservEvent::set_completed(bool flag)
{
    this->completed_ = flag;
}
*/



