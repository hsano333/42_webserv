#ifndef APPLICATION_FACTORY_HPP
#define APPLICATION_FACTORY_HPP
#include "config.hpp"
#include "request.hpp"
#include "cgi.hpp"
#include "cgi_application.hpp"
#include "server_application.hpp"
#include "get_application.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"
#include "method.hpp"
#include <string>

class ApplicationFactory
{
    public:
        ApplicationFactory(Config *cfg, CGI *cgi);
        ~ApplicationFactory();
        //Application* make_application(WebservApplicationEvent *event, IReader *ireader);
        Application* make_application(WebservEvent *event, IReader *ireader);
        //bool is_cgi();
        std::string get_target_path();
        std::string get_server_name();
    private:
        const ConfigServer *get_server(Request *req);
        void check_permission(Method &method);
        Config *cfg;
        CGI *cgi;

};

#endif
