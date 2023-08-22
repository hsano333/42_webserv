#ifndef WEBSERV_APPLICATION_HPP
#define WEBSERV_APPLICATION_HPP
#include "webserv_event.hpp"

class WebservApplication
{
    public:
        WebservApplication();
        ~WebservApplication();
        void process(WebservEvent *event);
    private:
};

#endif
