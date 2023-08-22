#ifndef WEBSERV_READER_HPP
#define WEBSERV_READER_HPP

#include "webserv_event.hpp"

class WebservReader
{
    public:
        WebservReader();
        ~WebservReader();
        void read(WebservEvent *event);
    private:


};

#endif
