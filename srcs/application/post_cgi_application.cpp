#include "post_cgi_application.hpp"
#include "http_exception.hpp"
#include "normal_writer.hpp"
#include "normal_reader.hpp"
#include "normal_file.hpp"
#include "directory_file.hpp"
#include "webserv_event.hpp"
#include "socket_writer.hpp"
#include "socket_reader.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
    NormalWriter *normal_writer = NormalWriter::get_instance();
    NormalReader *normal_reader = NormalReader::get_instance();

    Request *req = event->entity()->request();
    WebservFile *req_file = file_factory->make_request_file_read_buf(event->entity()->fd(), req);
    WebservFile *file = file_factory->make_socket_file_for_post_cgi(event->entity()->fd(), req_file, SocketWriter::get_instance(), SocketReader::get_instance());

    if(event->entity()->request()->header().is_chunked()){
        file = file_factory->make_socket_chunk_file_for_post_cgi(event->entity()->fd(), file);
    }

    WebservFile *from_socket_to_cgi_src = file;
    WebservFile *from_cgi_to_socket_dst = file_factory->make_result_file_for_cgi(event->entity()->fd(), event->entity()->app_result());
    ApplicationResult *result = event->entity()->app_result();
    WebservFile *result_file = file_factory->make_vector_file_for_cgi(event->entity()->fd(), MAX_BUF, result->pid());
    result->set_file(result_file);
    result->set_is_cgi(true);

    event->entity()->io().set_total_write_size(0);

    WebservFile *from_socket_to_cgi_dst = file_factory->make_socket_file(event->entity()->fd(), result->cgi_in(), normal_writer, NULL);
    WebservFile *from_cgi_to_socket_src = file_factory->make_socket_file(event->entity()->fd(), result->cgi_out(), NULL, normal_reader);

    event->entity()->io().set_read_io(from_socket_to_cgi_src, from_socket_to_cgi_dst);
    event->entity()->io().set_write_io(from_cgi_to_socket_src, from_cgi_to_socket_dst);

    event->entity()->io().set_read_fd(result->cgi_in());
    event->entity()->io().set_write_fd(result->cgi_out());

    return (event_factory->make_waiting_post_cgi(event));
}

E_EpollEvent PostCGIApplication::epoll_event(WebservEntity *entity)
{
    (void)entity;
    return (EPOLL_FOR_CGI_POST);
}

bool PostCGIApplication::execute(WebservEntity *entity)
{
    DEBUG("PostCGIApplication::execute");
    ApplicationResult *result = this->cgi->execute(entity, this->which());
    entity->set_result(result);

    return (true);
}


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

