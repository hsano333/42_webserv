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


WebservEvent* GetCGIApplication::next_event(WebservEvent *event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationWithCgiEvent::make_next_event");
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();

    Request *req = event->entity()->request();
    WebservFile *file = file_factory->make_vector_file(event->entity()->fd(), req->req_line().uri().query());
    req->set_file(file);
    WebservFile *write_src = file_factory->make_webserv_file_regular(event->entity()->fd(), event->entity()->request());
    WebservFile *read_dst = file_factory->make_webserv_file_regular(event->entity()->fd(), event->entity()->app_result());
    ApplicationResult *result = event->entity()->app_result();
    WebservFile *result_file = file_factory->make_vector_file(event->entity()->fd(), MAX_BUF);
    result->set_file(result_file);
    result->set_is_cgi(true);

    return (event_factory->make_io_socket_for_cgi(event, write_src, read_dst, result));
}

E_EpollEvent GetCGIApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    return (EPOLL_FOR_CGI);
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

