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
}

std::string ApplicationFactory::get_server_name()
{
    return ("");
}

std::string ApplicationFactory::get_target_path()
{
    return ("");
}

void ApplicationFactory::set_cgi(CGI *cgi_)
{
    if(this->cgi != NULL){
        ERROR("cgi is already existed");
        throw std::runtime_error("cgi is already existed");
    }
    this->cgi = cgi_;
}

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

    switch(method.to_enum())
    {
        case GET:
            if (is_cgi){
                DEBUG("ApplicationFactory::make_application() Get Method with CGI");
                app = GetCGIApplication::get_instance();
            }else{
                DEBUG("ApplicationFactory::make_application() Get Method without CGI");
                app = GetApplication::get_instance();
            }
            break;
        case POST:
            if (is_cgi){
                DEBUG("ApplicationFactory::make_application() Post Method with CGI");
                app = PostCGIApplication::get_instance();
            }else{
                DEBUG("ApplicationFactory::make_application() Post Method without CGI");
                app = PostApplication::get_instance();
            }
            break;
        case DELETE:
            DEBUG("ApplicationFactory::make_application() make Delete");
            app = DeleteApplication::get_instance();
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
    if(location->is_allowed_method(method) == false){
        throw HttpException("405");
    }
    return (app);
}

