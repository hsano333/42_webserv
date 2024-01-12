#ifndef DELETE_APPLICATION_HPP
#define DELETE_APPLICATION_HPP
#include "application.hpp"
#include "application_result.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "request.hpp"
#include "file.hpp"

class DeleteApplication : public Application
{
    public:
        //bool is_cgi();
        DeleteApplication();
        ~DeleteApplication();
        bool execute();
        ApplicationResult *get_result();
        //bool is_cgi() const;
        //static DeleteApplication* from_location(const Config *cfg, const Request *req, CGI *cgi);
        static DeleteApplication* from_location(const Config *cfg, const Request *req);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        //std::string &get_path_info();
        //std::string const &path_info() const;
        bool is_cgi() const;

        const Method &which() const;
        WebservCgiEvent *cgi_event();
    private:
        File *get_requested_file();

        //void execute_cgi();
        //void execute_not_cgi();
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        //CGI *cgi;
        //std::string cgi_application_path;
        //bool is_cgi_;
        std::map<std::string, std::string> tmp_headers;
        //std::string path_info_;
        //WebservCgiEvent cgi_event_;
        Method method;

        //StatusCode res_status_code;
        //std::string 
};
#endif
