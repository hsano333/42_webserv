#ifndef WEBSERV_MAKE_EVENT_HPP
#define WEBSERV_MAKE_EVENT_HPP
#include "file.hpp"
#include "webserv_event.hpp"

class WebservMakeEvent
{
    public:
        WebservEntity   *entity();
        void            set_file(File *file);
    private:
};

#endif
