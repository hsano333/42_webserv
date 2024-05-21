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
    DEBUG("WebservApplicationWithCgiEvent::make_next_event No.1");
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();

    //Request *req = event->entity()->request();
    //DEBUG("WebservApplicationWithCgiEvent::make_next_event No.2 query:" + req->req_line().uri().query());
    //WebservFile *file = file_factory->make_vector_file(event->entity()->fd(), req->req_line().uri().query());
    //DEBUG("WebservApplicationWithCgiEvent::make_next_event No.3");
    //req->set_file(file);

    //todo 
    //WebservFile *write_src = file_factory->make_request_file(event->entity()->fd(), event->entity()->request());

    // CGI: write_src( read from socket , write to cgi)
    WebservFile *write_src = NULL;

    // CGI: read_dst( read from CGI,  write to socket)
    WebservFile *read_dst = file_factory->make_result_file_for_cgi(event->entity()->fd(), event->entity()->app_result());
    ApplicationResult *result = event->entity()->app_result();
    WebservFile *result_file = file_factory->make_vector_file_for_cgi(event->entity()->fd(), MAX_BUF);
    result->set_file(result_file);
    result->set_is_cgi(true);

    return (event_factory->make_waiting_out_cgi(event, write_src, read_dst, result));
    //return (event_factory->make_io_socket_for_cgi(event, write_src, read_dst, result));
}

E_EpollEvent GetCGIApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    //return (EPOLL_FOR_CGI);
    return (EPOLL_WRITE);
}

bool GetCGIApplication::execute(WebservEntity *entity)
{
    DEBUG("GetCGIApplication::execute");
    //(void)entity;

    ApplicationResult *result = this->cgi->execute(entity, this->which());
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

