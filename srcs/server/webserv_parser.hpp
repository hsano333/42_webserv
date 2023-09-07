#ifndef WEBSERV_PARSER_HPP
#define WEBSERV_PARSER_HPP
#include "webserv_event.hpp"

class WebservParser
{
    public:
        WebservParser();
        ~WebservParser();
        void parse(WebservEvent *event);
    private:
};

#endif
