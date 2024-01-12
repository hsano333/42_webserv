#ifndef WEBSERV_MAKE_EVENT_HPP
#define WEBSERV_MAKE_EVENT_HPP
#include "file.hpp"
#include "webserv_event.hpp"

class IWebservMakeEvent
{
    public:
        virtual ~IWebservMakeEvent(){};
        virtual File *make() = 0;
        virtual void set_file(File *file) = 0;
        virtual void set_src(File *file) = 0;
        virtual void set_dst(File *file) = 0;
    private:
};

#endif
