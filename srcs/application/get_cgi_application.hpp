#ifndef GET_CGI_APPLICATION_HPP
#define GET_CGI_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "request.hpp"
#include "file.hpp"

class GetCGIApplication : public Application
{
    public:
        GetCGIApplication();
        ~GetCGIApplication();
        bool execute(WebservEvent *event);
        bool invoke(WebservEntity *entity);
        ApplicationResult *get_result();
        static GetCGIApplication* from_location(const Config *cfg, const Request *req, CGI *cgi);
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;
    private:
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        CGI *cgi;
        std::map<std::string, std::string> tmp_headers;
        std::string path_info_;

        Method method;
        ApplicationResult *result_;
};
#endif
