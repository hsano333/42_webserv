#include "get_cgi_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
//#include "cgi_file.hpp"
#include "directory_file.hpp"
#include "webserv_event.hpp"
#include <unistd.h>

GetCGIApplication::GetCGIApplication() : method(Method::from_string("GET"))
{
    ;
}

GetCGIApplication::~GetCGIApplication()
{
;
}

bool GetCGIApplication::is_cgi() const
{
    return (true);
}

bool GetCGIApplication::execute(WebservEntity *entity)
{
    (void)entity;
    Request *req = entity->request();
    Config const *cfg = entity->config();
    ConfigServer const *server = cfg->get_server(req);
    ConfigLocation const *location = cfg->get_location(server, req);
    req->set_path_info(location->root());
    //std::string path_info = location->root() + "/" + req->tmp_path_info();
    ApplicationResult *result = this->cgi->execute(location, req);
    entity->set_result(result);
    return (true);
}


/*
ApplicationResult *GetCGIApplication::get_result()
{
    return (this->result_);
}
*/

//GetCGIApplication* GetCGIApplication::from_location()
//{
    //GetCGIApplication *app = new GetCGIApplication();


    /*
    app->cfg = cfg;
    app->server = cfg->get_server(req);
    app->location = cfg->get_location(app->server, req);
    app->req = req;
    app->cgi = cgi;
    app->path_info_ = app->location->root() + "/" + app->req->tmp_path_info();
    */
    //return (app);
//}


const Method &GetCGIApplication::which() const
{
    return (this->method);
}

GetCGIApplication *GetCGIApplication::singleton = NULL;
GetCGIApplication *GetCGIApplication::get_instance()
{
    if (GetCGIApplication::singleton == NULL){
        singleton = new GetCGIApplication();
    }
    return (singleton);
}

