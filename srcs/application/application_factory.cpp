#include "global.hpp"
#include "application_factory.hpp"
#include "application.hpp"
#include "server_application.hpp"
#include "cgi_application.hpp"
#include "get_application.hpp"
#include "method.hpp"
#include "http_exception.hpp"
//#include "post_application.hpp"
//#include "delete_application.hpp"

ApplicationFactory::ApplicationFactory(Config *cfg, CGI *cgi) 
    : 
        cfg(cfg),
        cgi(cgi)
{
    ;
}

ApplicationFactory::~ApplicationFactory()
{
}

std::string ApplicationFactory::get_server_name()
{
    return ("");
}

/*
bool ApplicationFactory::is_cgi()
{
    return (true);
}
*/

std::string ApplicationFactory::get_target_path()
{
    return ("");
}


/*
const ConfigServer *ApplicationFactory::get_server(Request *req)
{
    Header const &header = req->header();

    std::string const &header_hostname = header.get_host();
    Split sp(header_hostname, ":");
    if(sp.size() != 2){
        ERROR("There is no Host in Headers");
        throw HttpException("400");
    }
    std::string name = sp[0];
    std::string port_str = sp[1];
    Port port = Port::from_string(port_str);

    const ConfigServer *server = this->cfg->get_server(port, name);
    return (server);

}
*/


Application* ApplicationFactory::make_application(Request *req)
{
    Application* app;
    RequestLine const &req_line = req->req_line();
    const ConfigServer *server = cfg->get_server(req);
    const ConfigLocation *location= this->cfg->get_location(server, req);
    req->set_requested_filepath(location);

    Method const &method = req_line.method();
    switch(method.to_enum())
    {
        case GET:
            DEBUG("ApplicationFactory::make_application() make Get");
            app = GetApplication::from_location(cfg, req, cgi);
            break;
        case POST:
            //todo
            DEBUG("ApplicationFactory::make_application() make Post");
            app = GetApplication::from_location(cfg, req, cgi);
            break;
        case DELETE:
            //todo
            DEBUG("ApplicationFactory::make_application() make Delete");
            app = GetApplication::from_location(cfg, req, cgi);
            break;
        default:
            ERROR("ApplicationFactory::make_application(): Invalid method");;
            std::runtime_error("invalid Method");
    }

    return (app);
}

