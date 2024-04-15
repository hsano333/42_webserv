#include "webserv_event.hpp"
#include "global.hpp"
#include "socket_reader.hpp"

void WebservEvent::update_time()
{
    this->updated_time_ = std::time(NULL);
}

std::time_t WebservEvent::last_updated_time()
{
    return (this->updated_time_);
}

// if time is exceed, return true;
bool WebservEvent::check_timeout(std::time_t now)
{
    DEBUG("WebservEvent::check_timeout diff=" + Utility::to_string(now - this->updated_time_));
    std::time_t diff = now - this->updated_time_;
    return (diff >= TIMEOUT);
}

