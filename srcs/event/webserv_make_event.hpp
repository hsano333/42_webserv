#ifndef WEBSERV_MAKE_EVENT_HPP
#define WEBSERV_MAKE_EVENT_HPP
#include "file.hpp"
#include "webserv_event.hpp"

class IWebservMakeEvent : public WebservEvent
{
    public:
        virtual ~IWebservMakeEvent(){};
        virtual File *make() = 0;
        virtual void set_file(File *file) = 0;
    private:
};

#endif
