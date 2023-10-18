#ifndef GET_APPLICATION_HPP
#define GET_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"

class GetApplication : public Application
{
    public:
        //bool is_cgi();
        GetApplication();
        ~GetApplication();
        void execute();
        //bool is_cgi() const;
        void check_permission();
        //static GetApplication* from_location(const ConfigLocation *location, const Request *req, CGI *cgi);
        static GetApplication* from_location(const Config *cfg, const Request *req, CGI *cgi);
        Response* make_response();
        std::string const &get_requested_path() const;
    private:

        void execute_cgi();
        void execute_not_cgi();
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        CGI *cgi;
        std::string cgi_application_path;
        bool is_cgi;

        //StatusCode res_status_code;
        //std::string 
};
#endif
