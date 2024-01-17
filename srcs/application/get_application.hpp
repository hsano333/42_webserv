#ifndef GET_APPLICATION_HPP
#define GET_APPLICATION_HPP
#include "application.hpp"
#include "application_result.hpp"
//#include "cgi.hpp"
#include "config_location.hpp"
#include "config.hpp"
//#include "webserv_cgi_event.hpp"
#include "request.hpp"
#include "file.hpp"
#include "status_code.hpp"

class GetApplication : public Application
{
    public:
        //bool is_cgi();
        GetApplication();
        ~GetApplication();
        bool execute(WebservEvent *event);
        ApplicationResult *get_result();
        //bool is_cgi() const;
        //void check_permission();
        //static GetApplication* from_location(const ConfigLocation *location, const Request *req, CGI *cgi);
        static GetApplication* from_location(const Config *cfg, const Request *req);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        //std::string &get_path_info();
        //std::string const &path_info() const;
        bool is_cgi() const;
        const Method &which() const;

        WebservCgiEvent *cgi_event();

        //method for application class
        StatusCode const &code() const;
        std::map<string, string> const &header() const;
    private:

        string get_index_path(Request *req, bool *is_existed);
        File *get_requested_file();
        //bool execute_cgi();
        //bool execute_not_cgi();
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        //CGI *cgi;
        //FileDiscriptor cgi_in;
        //FileDiscriptor cgi_out;
        //std::string cgi_application_path;
        //bool is_cgi_;
        std::map<std::string, std::string> tmp_headers;
        std::string path_info_;
        Method method;
        string check_content(string const &filepath);

        //StatusCode code_;
        std::map<std::string, std::string> header_;
        ApplicationResult *result_;

};
#endif
