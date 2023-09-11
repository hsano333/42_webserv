
#ifndef WEBSERV_EVENT_WRITE_HPP
#define WEBSERV_EVENT_WRITE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "iwriter.hpp"
#include "response.hpp"

class WebservWriteEvent : public WebservEvent
{
    public:
        WebservWriteEvent();
        WebservWriteEvent(FileDiscriptor fd);
        WebservWriteEvent(FileDiscriptor fd, IWriter* writer);
        ~WebservWriteEvent();
        EWebservEvent which();

        FileDiscriptor get_fd();
        void increase_and_check_timeout_count(int count);
        int write(char const *buf, size_t size);
        static WebservWriteEvent *from_fd(FileDiscriptor fd);
        Response *res;

    private:
        FileDiscriptor fd;
        int timeout_count;
        IWriter *writer;
};
#endif
