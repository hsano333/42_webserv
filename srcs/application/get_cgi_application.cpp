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
    DEBUG("WebservApplicationWithCgiEvent::make_next_event");
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *from_socket_to_cgi_src = NULL;
    WebservFile *from_cgi_to_socket_dst = file_factory->make_result_file_for_cgi(event->entity()->fd(), event->entity()->app_result());
    ApplicationResult *result = event->entity()->app_result();
    WebservFile *result_file = file_factory->make_vector_file_for_cgi(event->entity()->fd(), MAX_BUF, result->pid());
    result->set_file(result_file);
    result->set_is_cgi(true);

    NormalReader *normal_reader = NormalReader::get_instance();

    // CGI I/O
    WebservFile *from_socket_to_cgi_dst = NULL;
    WebservFile *from_cgi_to_socket_src = file_factory->make_socket_file(event->entity()->fd(), result->cgi_out(), NULL, normal_reader);

    event->entity()->io().set_read_io(from_socket_to_cgi_src, from_socket_to_cgi_dst);
    event->entity()->io().set_write_io(from_cgi_to_socket_src, from_cgi_to_socket_dst);

    event->entity()->io().set_read_fd(result->cgi_in());
    event->entity()->io().set_write_fd(result->cgi_out());

    return (event_factory->make_waiting_get_cgi(event));
}

E_EpollEvent GetCGIApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    return (EPOLL_FOR_CGI_WAIT_CGI);
}

bool GetCGIApplication::execute(WebservEntity *entity)
{
    DEBUG("GetCGIApplication::execute");

    ApplicationResult *result = this->cgi->execute(entity, this->which());
    entity->set_result(result);
    return (true);
}

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

