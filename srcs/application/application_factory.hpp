#ifndef APPLICATION_FACTORY_HPP
#define APPLICATION_FACTORY_HPP
#include "config.hpp"
#include "request.hpp"
#include "cgi_application.hpp"
#include "server_application.hpp"
#include "get_application.hpp"
#include "method.hpp"
#include <string>

class ApplicationFactory
{
    public:
        ApplicationFactory(Config *cfg);
        ~ApplicationFactory();
        Application* make_application(Request *req);
        bool is_cgi();
        std::string get_target_path();
        std::string get_server_name();
    private:
        ConfigServer const *get_server(Request *req);
        Config *cfg;

};

#endif
