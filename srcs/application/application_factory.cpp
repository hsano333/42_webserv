#include "global.hpp"
#include "application_factory.hpp"
#include "application.hpp"
#include "server_application.hpp"
#include "cgi_application.hpp"
#include "get_application.hpp"
#include "get_cgi_application.hpp"
#include "post_application.hpp"
#include "post_cgi_application.hpp"
#include "delete_application.hpp"
#include "method.hpp"
#include "http_exception.hpp"
//#include "post_application.hpp"
//#include "delete_application.hpp"

ApplicationFactory::ApplicationFactory() : cgi(NULL)
{
    ;
}


ApplicationFactory *ApplicationFactory::singleton = NULL;
ApplicationFactory *ApplicationFactory::get_instance()
{
    if (ApplicationFactory::singleton == NULL){
        singleton = new ApplicationFactory();
    }
    return (singleton);
}

ApplicationFactory::~ApplicationFactory()
{
    delete (singleton);
}

std::string ApplicationFactory::get_server_name()
{
    return ("");
}

/*
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



void ApplicationFactory::set_cgi(CGI *cgi_)
{
    if(this->cgi != NULL){
        ERROR("cgi is already existed");
        throw std::runtime_error("cgi is already existed");
    }
    this->cgi = cgi_;
}

//Application* ApplicationFactory::make_application(WebservApplicationEvent *event, IReader *ireader)
Application* ApplicationFactory::make_application(WebservEntity *entity)
{
    DEBUG("ApplicationFactory::make_application()");
    Application* app;
    const Config *cfg = entity->config();
    Request *req = entity->request();
    RequestLine const &req_line = req->req_line();
    Method const &method = req_line.method();

    const ConfigLocation *location = cfg->get_location(cfg->get_server(req), req);
    bool is_cgi = cgi->check_cgi_application_path(req, location);
    if(location->is_allowed_method(method) == false){
        throw HttpException("405");
    }

    switch(method.to_enum())
    {
        case GET:
            if (is_cgi){
                DEBUG("ApplicationFactory::make_application() Get Method with CGI");
                app = GetCGIApplication::from_location(cfg, req, cgi);
            }else{
                DEBUG("ApplicationFactory::make_application() Get Method with not CGI");
                app = GetApplication::get_instance();
            }
            break;
        case POST:
            //todo
            if (is_cgi){
                DEBUG("ApplicationFactory::make_application() Post Method with CGI");
                app = NULL;
                //app = PostCGIApplication::from_location(cfg, req, cgi);
                //app = PostCGIApplication::from_location(cfg, event, cgi);
            }else{
                app = NULL;
                DEBUG("ApplicationFactory::make_application() Post Method with not CGI");
                //app = PostApplication::from_location(cfg, event);
            }
            break;
        case DELETE:
            DEBUG("ApplicationFactory::make_application() make Delete");
            app = DeleteApplication::from_location(cfg, req);
            break;
        case PUT:
        case CONNECT:
        case OPTIONS:
        case TRACE:
        case PATCH:
        case HEAD:
        default:
            ERROR("ApplicationFactory::make_application(): Invalid method");;
            throw HttpException("501");
    }
    return (app);
}

