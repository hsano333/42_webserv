
#ifndef WEBSERV_EVENT_NOTHING_HPP
#define WEBSERV_EVENT_NOTHING_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"

class WebservNothingEvent : public WebservEvent
{
    public:
        WebservNothingEvent();
        ~WebservNothingEvent();
        EWebservEvent which();

        static WebservNothingEvent *from_fd(FileDiscriptor fd);
        FileDiscriptor get_fd();
        void increase_and_check_timeout_count(int count);
    private:
};
#endif
