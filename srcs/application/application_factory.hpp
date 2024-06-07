#ifndef APPLICATION_FACTORY_HPP
#define APPLICATION_FACTORY_HPP
#include "config.hpp"
#include "request.hpp"
#include "cgi.hpp"
#include "method.hpp"
#include "application.hpp"
#include <string>

class ApplicationFactory
{
    public:
        ~ApplicationFactory();
        static ApplicationFactory *get_instance();
        Application* make_application(WebservEntity *entity);
        void set_cgi(CGI *cgi);
        std::string get_target_path();
        std::string get_server_name();
    private:
        ApplicationFactory();
        static ApplicationFactory *singleton;
        const ConfigServer *get_server(Request *req);
        void check_permission(Method &method);
        CGI *cgi;

};

#endif
