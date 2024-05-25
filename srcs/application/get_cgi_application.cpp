#include "get_cgi_application.hpp"
#include "http_exception.hpp"
#include "normal_writer.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
//#include "cgi_file.hpp"
#include "directory_file.hpp"
#include "webserv_event.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    WebservFile *from_socket_to_cgi_src = NULL;

    // CGI: read_dst( read from CGI,  write to socket)
    WebservFile *from_cgi_to_socket_dst = file_factory->make_result_file_for_cgi(event->entity()->fd(), event->entity()->app_result());
    ApplicationResult *result = event->entity()->app_result();
    WebservFile *result_file = file_factory->make_vector_file_for_cgi(event->entity()->fd(), MAX_BUF);
    result->set_file(result_file);
    result->set_is_cgi(true);

    //NormalWriter *normal_writer = NormalWriter::get_instance();
    NormalReader *normal_reader = NormalReader::get_instance();

    // CGI I/O
    //WebservFile *write_dst = file_factory->make_socket_file(result->cgi_in(), normal_writer, NULL);
    WebservFile *from_socket_to_cgi_dst = NULL;
    WebservFile *from_cgi_to_socket_src = file_factory->make_socket_file(result->cgi_out(), NULL, normal_reader);

    event->entity()->io().set_read_io(from_socket_to_cgi_src, from_socket_to_cgi_dst);
    event->entity()->io().set_write_io(from_cgi_to_socket_src, from_cgi_to_socket_dst);

    event->entity()->io().set_read_fd(result->cgi_in());
    event->entity()->io().set_write_fd(result->cgi_out());


    /*
    int wstatus;
    ERROR("Child Process ERROR pid:" +  Utility::to_string(event->entity()->app_result()->pid().to_int()));
    int result_exe = waitpid(event->entity()->app_result()->pid().to_int(), &wstatus,  WUNTRACED | WNOHANG);
    ERROR("Child Process ERROR result:" +  Utility::to_string(result_exe));

    if(result_exe == -1){
        ERROR("Child Process ERROR");
        throw HttpException("500");
    }
        //flag = true;
        if(WIFEXITED(wstatus)){
            DEBUG("exited, status=" + Utility::to_string(WEXITSTATUS(wstatus)));
        } else if (WIFSIGNALED(wstatus)) {
            DEBUG("killed by  status=" + Utility::to_string(WTERMSIG(wstatus)));
        } else if (WIFSTOPPED(wstatus)) {
            DEBUG("stopped by signal =" + WSTOPSIG(wstatus));
        } else if (WIFCONTINUED(wstatus)) {
            DEBUG("continued\n");
        }
        */





    return (event_factory->make_waiting_get_cgi(event));
    //return (event_factory->make_waiting_out_cgi(event, write_src, read_dst, result));
    //return (event_factory->make_io_socket_for_cgi(event, write_src, read_dst, result));
}

E_EpollEvent GetCGIApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    return (EPOLL_FOR_CGI_GET);
    //return (EPOLL_WRITE);
}

bool GetCGIApplication::execute(WebservEntity *entity)
{
    DEBUG("GetCGIApplication::execute");
    //(void)entity;
    //


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

