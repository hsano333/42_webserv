#ifndef WEBSERV_EVENT_READ_HPP
#define WEBSERV_EVENT_READ_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "ireader.hpp"

class WebservReadEvent : public WebservEvent
{
    public:
        WebservReadEvent();
        WebservReadEvent(FileDiscriptor fd);
        WebservReadEvent(FileDiscriptor fd, IReader* ireader);
        ~WebservReadEvent();
        EWebservEvent which();

        FileDiscriptor get_fd();
        void increase_and_check_timeout_count(int count);
        int read(char *buf, size_t size);


    private:
        FileDiscriptor fd;
        EWebservEvent event_type;
        int timeout_count;
        IReader *ireader;
};
#endif
