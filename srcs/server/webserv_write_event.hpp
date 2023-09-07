
#ifndef WEBSERV_EVENT_WRITE_HPP
#define WEBSERV_EVENT_WRITE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"

class WebservWriteEvent : public WebservEvent
{
    public:
        WebservWriteEvent();
        WebservWriteEvent(FileDiscriptor fd);
        ~WebservWriteEvent();
        EWebservEvent which();

        FileDiscriptor get_fd();
        void increase_and_check_timeout_count(int count);

    private:
        FileDiscriptor fd;
        EWebservEvent event_type;
        int timeout_count;
};
#endif
