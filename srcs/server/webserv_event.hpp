#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP

typedef enum E_WebservEvent
{
    READ_EVENT,
    APPLICATION_EVENT,
    WRITE_EVENT,
} EWebservEvent;

class WebservEvent
{
    public:
        WebservEvent();
        ~WebservEvent();
        EWebservEvent which();

    private:


};

#endif
