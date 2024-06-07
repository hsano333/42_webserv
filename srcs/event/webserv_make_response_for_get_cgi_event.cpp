#include "webserv_make_response_for_get_cgi_event.hpp"
#include "socket_writer.hpp"
#include "socket_file.hpp"
#include "response.hpp"
#include "error_file.hpp"
#include "normal_reader.hpp"
#include <assert.h>


WebservMakeResponseForGetCGIEvent::WebservMakeResponseForGetCGIEvent()
{
    ;
}

WebservMakeResponseForGetCGIEvent::~WebservMakeResponseForGetCGIEvent()
{
    ;
}

WebservMakeResponseForGetCGIEvent *WebservMakeResponseForGetCGIEvent::singleton = NULL;
WebservMakeResponseForGetCGIEvent *WebservMakeResponseForGetCGIEvent::get_instance()
{
    if (WebservMakeResponseForGetCGIEvent::singleton == NULL){
        singleton = new WebservMakeResponseForGetCGIEvent();
    }
    return (singleton);
}

void WebservMakeResponseForGetCGIEvent::delete_myself()
{
    if(WebservMakeResponseForGetCGIEvent::singleton)
        delete WebservMakeResponseForGetCGIEvent::singleton;
}

WebservEvent *WebservMakeResponseForGetCGIEvent::from_event(WebservEvent *event)
{
    DEBUG("WebservMakeResponseForGetCGIEvent::from_fd");
    WebservMakeResponseForGetCGIEvent *res_event = WebservMakeResponseForGetCGIEvent::get_instance();
    WebservEvent *new_event = new WebservEvent( res_event, io_work_reverse_io<WebservMakeResponseForGetCGIEvent>, event->entity());
    return (new_event);
}


Response* WebservMakeResponseForGetCGIEvent::make_response_for_cgi(ApplicationResult *result, WebservEntity *entity)
{
    DEBUG("WebservMakeResponseForGetCGIEvent::make_response_for_cgi()");

    char *data;
    int read_size = result->file()->read(&data, MAX_READ_SIZE);

    if(read_size <= 0){
        ERROR("CGI Respons Data is nothing or error");
        throw HttpException("500");
    }

    char *body_p = Utility::strnstr(data, NL2_CGI, read_size);
    if(!body_p){
        ERROR("CGI Statement Error: not find LFLF(double New Line)");
        throw HttpException("500");
    }

    size_t header_size = body_p - data;
    if(header_size >= MAX_REAUEST_EXCEPT_BODY){
        ERROR("exceed CGI Response Status Line" + Utility::to_string(header_size));
        throw HttpException("exceed CGI Response Status Line:" + Utility::to_string(header_size));
    }
    *body_p = '\0';


    Split headers_line(data, NL_CGI, false, true);
    IReader *reader = NormalReader::get_instance();

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *file = file_factory->make_pipe_file(entity->fd(),  result->cgi_out(), reader);
    Request const *req = entity->request();
    ConfigServer const *server = entity->config()->get_server(req);
    Response *res = Response::from_cgi_header_line(entity->fd(), headers_line, file);

    int tmp_size = read_size - header_size - NL_OFFSET;
    res->set_buf_body(body_p + NL_OFFSET, tmp_size);
    if(!res->check_body_size(server))
    {
        delete res;
        throw HttpException("500");
    }


    if(res->header().get_content_length() == -1)
    {
        if(res->header().is_chunked() == false){
            res->add_header(TRANSFER_ENCODING, TRANSFER_ENCODING_CHUNKED);
        }
    }

    res->check_body_and_chunk();
    if(!res->has_body()){
        res->add_header(CONTENT_LENGTH, "0");
    }
    if(res->is_chunk()){
        WebservFileFactory *file_factory = WebservFileFactory::get_instance();
        WebservFile *socket_file = file_factory->make_socket_chunk_file_for_write(entity->fd(), res->get_file(), res->buffer());
        res->clear_buffer();
        socket_file->set_chunk(true);
        res->switching_file(socket_file);
    }
    return (res);

}

WebservEvent* WebservMakeResponseForGetCGIEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservMakeResponseForGetCGIEvent::make_next_event()");
    WebservEntity *entity = event->entity();
    ApplicationResult *result = event->entity()->app_result();
    Response *res = make_response_for_cgi(result, entity);
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *res_file = file_factory->make_webserv_file_regular(event->entity()->fd(), res);

    entity->set_response(res);

    WebservFile *read_dst = file_factory->make_socket_file(event->entity()->fd(), event->entity()->fd(), SocketWriter::get_instance(), NULL);
    entity->io().set_write_io(res_file, read_dst);

    return (event_factory->make_io_socket_for_get_cgi(event));
}

E_EpollEvent WebservMakeResponseForGetCGIEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    DEBUG("WebservMakeResponseForGetCGIEvent::epoll_event()");

    if (event->entity()->completed()){
        return (EPOLL_WRITE);
    }
    return (EPOLL_FOR_CGI_WAIT_CGI);

}

void WebservMakeResponseForGetCGIEvent::check_completed(WebservEntity * entity)
{
    WebservFile *dst = entity->io().destination_for_write();
    bool flag = dst->completed();
    DEBUG("WebservMakeResponseForGetCGIEvent::check_completed flag:" + Utility::to_string(flag));
    entity->set_completed(flag);
}
