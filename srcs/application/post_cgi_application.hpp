#ifndef POST_CGI_APPLICATION_HPP
#define POST_CGI_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "file.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"

class PostCGIApplication : public Application
{
    public:
        PostCGIApplication();
        ~PostCGIApplication();
        void init(WebservEvent *event);
        bool execute(WebservEvent *event);
        bool invoke(WebservEntity *entity);
        ApplicationResult *get_result();
        void check_permission();
        static PostCGIApplication* from_location(const Config *cfg, WebservEvent *event, CGI *cgi);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;
    private:

        File *get_requested_file();
        WebservEvent *event;
        bool execute_cgi();
        bool execute_not_cgi();
        bool check_not_cgi_end(size_t sum);
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        Response *res;
        CGI *cgi;
        std::map<std::string, std::string> tmp_headers;
        std::string path_info_;
        Method method;
};
#endif
