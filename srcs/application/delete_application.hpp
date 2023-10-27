#ifndef DELETE_APPLICATION_HPP
#define DELETE_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "file.hpp"

class DeleteApplication : public Application
{
    public:
        //bool is_cgi();
        DeleteApplication();
        ~DeleteApplication();
        void execute();
        //bool is_cgi() const;
        void check_permission();
        //static DeleteApplication* from_location(const Config *cfg, const Request *req, CGI *cgi);
        static DeleteApplication* from_location(const Config *cfg, const Request *req);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        //std::string &get_path_info();
        //std::string const &path_info() const;
        bool is_cgi() const;
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

        //StatusCode res_status_code;
        //std::string 
};
#endif
