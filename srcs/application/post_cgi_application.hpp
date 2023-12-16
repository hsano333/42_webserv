#ifndef POST_CGI_APPLICATION_HPP
#define POST_CGI_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "file.hpp"
#include "webserv_application_event.hpp"

class PostCGIApplication : public Application
{
    public:
        //bool is_cgi();
        PostCGIApplication();
        ~PostCGIApplication();
        bool execute();
        //bool is_cgi() const;
        void check_permission();
        //static PostCGIApplication* from_location(const ConfigLocation *location, const Request *req, CGI *cgi);
        static PostCGIApplication* from_location(const Config *cfg, WebservApplicationEvent *event, IReader *reader, CGI *cgi);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        //std::string &get_path_info();
        //std::string const &path_info() const;
        bool is_cgi() const;

        WebservCgiEvent *cgi_event();
    private:

        File *get_requested_file();
        //File *file;
        WebservApplicationEvent *event;
        bool execute_cgi();
        bool execute_not_cgi();
        bool check_not_cgi_end(size_t sum);

        //bool is_continued;
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        Response *res;
        CGI *cgi;
        //std::string cgi_application_path;
        //bool is_cgi_;
        std::map<std::string, std::string> tmp_headers;
        std::string path_info_;
        IReader *reader;
        WebservCgiEvent *cgi_event_;
};
#endif