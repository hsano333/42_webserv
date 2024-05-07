#include "post_cgi_application.hpp"
#include "http_exception.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
//#include "cgi_file.hpp"
#include "directory_file.hpp"
#include "webserv_event.hpp"
#include "socket_writer.hpp"
#include "socket_reader.hpp"
#include <unistd.h>

PostCGIApplication::PostCGIApplication() : method(Method::from_string("POST"))
{
    ;
}

PostCGIApplication::~PostCGIApplication()
{
;
}

bool PostCGIApplication::is_cgi() const
{
    return (true);
}


WebservEvent* PostCGIApplication::next_event(WebservEvent *event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationWithCgiEvent::make_next_event No.1");
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();

    Request *req = event->entity()->request();
    WebservFile *req_file = file_factory->make_request_file_read_buf(event->entity()->fd(), req);
    WebservFile *file = file_factory->make_pipe_file(event->entity()->fd(), req_file, SocketWriter::get_instance(), SocketReader::get_instance());

    if(event->entity()->request()->header().is_chunked()){
        file = file_factory->make_socket_chunk_file(event->entity()->fd(), file);
    }

    //todo 
    /*
    WebservFile *write_src = file;
    WebservFile *read_dst = file_factory->make_webserv_file_regular(event->entity()->fd(), event->entity()->app_result());
    */
    ApplicationResult *result = event->entity()->app_result();
    WebservFile *result_file = file_factory->make_vector_file_for_cgi(event->entity()->fd(), MAX_BUF);
    result->set_file(result_file);
    result->set_is_cgi(true);

    //todo
    //return (event_factory->make_io_socket_for_cgi(event, write_src, read_dst, result));
    return (event_factory->make_io_socket_for_cgi(event));
}

E_EpollEvent PostCGIApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    return (EPOLL_FOR_CGI);
}

bool PostCGIApplication::execute(WebservEntity *entity)
{
    DEBUG("PostCGIApplication::execute");
    //(void)entity;
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
ApplicationResult *PostCGIApplication::get_result()
{
    return (this->result_);
}
*/

//PostCGIApplication* PostCGIApplication::from_location()
//{
    //PostCGIApplication *app = new PostCGIApplication();


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


const Method &PostCGIApplication::which() const
{
    return (this->method);
}

PostCGIApplication *PostCGIApplication::singleton = NULL;
PostCGIApplication *PostCGIApplication::get_instance()
{
    if (PostCGIApplication::singleton == NULL){
        singleton = new PostCGIApplication();
    }
    return (singleton);
}

