#ifndef GET_APPLICATION_HPP
#define GET_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "file.hpp"

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
        File *get_requested_file();
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
        std::map<std::string, std::string> tmp_headers;

        //StatusCode res_status_code;
        //std::string 
};
#endif
