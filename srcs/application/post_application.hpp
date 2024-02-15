#ifndef POST_APPLICATION_HPP
#define POST_APPLICATION_HPP
#include "application.hpp"
//#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "file.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"

class PostApplication : public Application
{
    public:
        //bool is_cgi();
        PostApplication();
        ~PostApplication();
        bool execute(WebservEvent *event);
        bool invoke(WebservEntity *entity);
        ApplicationResult *get_result();
        //bool is_cgi() const;
        //static PostApplication* from_location(const ConfigLocation *location, const Request *req, CGI *cgi);
        static PostApplication* from_location(const Config *cfg, WebservEvent *event);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        //std::string &get_path_info();
        //std::string const &path_info() const;
        bool is_cgi() const;
        const Method &which() const;

        WebservCgiEvent *cgi_event();
    private:

        File *get_requested_file();
        //File *file;
        WebservEvent *event;
        bool check_not_cgi_end(size_t sum);

        //bool is_continued;
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        Response *res;
        //CGI *cgi;
        //std::string cgi_application_path;
        //bool is_cgi_;
        std::map<std::string, std::string> tmp_headers;
        //std::string path_info_;
        //IReader *reader;
        Method method;
        //WebservCgiEvent cgi_event_;
};
#endif
