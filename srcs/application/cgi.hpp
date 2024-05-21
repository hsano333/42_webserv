#ifndef CGI_HPP
#define CGI_HPP
#include "request.hpp"
//#include "application.hpp"
#include "application_result.hpp"
#include "webserv_entity.hpp"

class CGI
{
    public:
        CGI();
        ~CGI();
        //bool is_cgi(const Request *req, const ConfigLocation *location) const;
        bool check_cgi_application_path(const Request *req, const ConfigLocation *location) const;
        int make_thread(int* fd_in, int* fd_out);
        //ApplicationResult *execute(ConfigLocation const *location, Request const *req);
        ApplicationResult *execute(WebservEntity *entity, const Method &method);
    private:

};

#endif
