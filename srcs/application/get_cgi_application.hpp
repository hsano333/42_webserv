#ifndef GET_CGI_APPLICATION_HPP
#define GET_CGI_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "webserv_cgi_event.hpp"
#include "request.hpp"
#include "file.hpp"

class GetCGIApplication : public Application
{
    public:
        //bool is_cgi();
        GetCGIApplication();
        ~GetCGIApplication();
        bool execute(WebservEvent *event);
        ApplicationResult *get_result();
        //bool is_cgi() const;
        //static GetCGIApplication* from_location(const ConfigLocation *location, const Request *req, CGI *cgi);
        static GetCGIApplication* from_location(const Config *cfg, const Request *req, CGI *cgi);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        //std::string &get_path_info();
        //std::string const &path_info() const;
        bool is_cgi() const;

        const Method &which() const;
        WebservCgiEvent *cgi_event();
    private:

        File *get_requested_file();
        //bool execute_cgi();
        //bool execute_not_cgi();
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        CGI *cgi;
        //FileDiscriptor cgi_in;
        //FileDiscriptor cgi_out;
        //std::string cgi_application_path;
        //bool is_cgi_;
        std::map<std::string, std::string> tmp_headers;
        std::string path_info_;

        WebservCgiEvent *cgi_event_;
        Method method;


        //StatusCode res_status_code;
        //std::string 
};
#endif
