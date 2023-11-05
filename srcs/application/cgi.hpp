#ifndef CGI_HPP
#define CGI_HPP
#include "request.hpp"

class CGI
{
    public:
        CGI();
        ~CGI();
        //bool is_cgi(const Request *req, const ConfigLocation *location) const;
        void check_cgi_application_path(const Request *req, const ConfigLocation *location) const;
        int make_thread(int* fd_in, int* fd_out);
    private:

};

#endif
