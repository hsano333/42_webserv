#ifndef CGI_HPP
#define CGI_HPP
#include "request.hpp"

class CGI
{
    public:
        CGI();
        ~CGI();
        //bool is_cgi(const Request *req, const ConfigLocation *location) const;
        std::string const &get_cgi_application_path(const Request *req, const ConfigLocation *location) const;
    private:

};

#endif
