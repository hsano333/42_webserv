
#ifndef WEBSERV_EVENT_WRITE_HPP
#define WEBSERV_EVENT_WRITE_HPP
#include "webserv_event.hpp"
#include "file_discriptor.hpp"
#include "iwriter.hpp"
#include "response.hpp"
#include "file.hpp"

class WebservWriteEvent : public WebservEvent
{
    public:
        WebservWriteEvent();
        WebservWriteEvent(FileDiscriptor fd);
        WebservWriteEvent(FileDiscriptor fd, Request *req, Response *res, IWriter* writer);
        ~WebservWriteEvent();
        EWebservEvent which();

        FileDiscriptor  fd();
        Request         *req();
        Response        *res();
        void increase_timeout_count(int count);
        int  timeout_count();
        int write(char const *buf, size_t size);
        static WebservWriteEvent *from_error_status_code(WebservEvent *event, StatusCode &code, File *file, IWriter *writer);
        static WebservWriteEvent *from_event(WebservEvent *event, Response *res, IWriter *writer);

    private:
        FileDiscriptor  fd_;
        Request         *req_;
        Response        *res_;
        int             timeout_count_;
        IWriter *writer;
};
#endif
