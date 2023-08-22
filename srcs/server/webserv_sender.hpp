#ifndef WEBSERV_SENDER_HPP
#define WEBSERV_SENDER_HPP
#include "webserv_event.hpp"
class WebservSender
{
    public:
        WebservSender();
        ~WebservSender();
        void send(WebservEvent *event);
    private:
};

#endif
