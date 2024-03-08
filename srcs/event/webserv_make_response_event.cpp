#include "webserv_make_response_event.hpp"
#include "webserv_event.hpp"
//#include "webserv_io_event.hpp"
#include "http_exception.hpp"
#include "socket_file.hpp"
#include "application_result.hpp"
#include "header_word.hpp"
#include "normal_reader.hpp"

WebservMakeResponseEvent::WebservMakeResponseEvent()
{
    ;
}

WebservMakeResponseEvent::~WebservMakeResponseEvent()
{
};


Response* WebservMakeResponseEvent::make_response_for_cgi(ApplicationResult *result, WebservEntity *entity)
{
    DEBUG("WebservMakeResponseEvent::make_response_for_cgi()");

    char *data;
    int read_size = result->file()->read(&data, MAX_READ_SIZE);

    if(read_size <= 0){
        ERROR("CGI Respons Data is nothing or error");
        throw HttpException("500");
    }

    // RFC3875にはNL(New Line)の具体的な定義がないため、
    // 改行文字はLFとする(Linuxでの運用を想定しているため)
    char *body_p = Utility::strnstr(data, LF2, read_size);
    if(!body_p){
        ERROR("CGI Statement Error: not find LFLF(double New Line)");
        throw HttpException("500");
    }

    size_t header_size = body_p - data;
    if(header_size >= MAX_STATUS_LINE){
        ERROR("exceed CGI Response Status Line" + Utility::to_string(header_size));
        throw HttpException("exceed CGI Response Status Line:" + Utility::to_string(header_size));
    }
    *body_p = '\0';

    Split headers_line(data, LF, false, true);
    IReader *reader = NormalReader::get_instance();

    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *file = file_factory->make_socket_file(result->cgi_out(), NULL, reader );
    Request const *req = entity->request();
    ConfigServer const *server = entity->config()->get_server(req);
    Response *res = Response::from_cgi_header_line(headers_line, file);

    int tmp_size = read_size - header_size - 4;
    res->set_buf_body(body_p + 4, tmp_size);
    if(!res->check_body_size(server))
    {
        delete res;
        throw HttpException("500");
    }

    return (res);

}

Response* WebservMakeResponseEvent::make_response(ApplicationResult *result)
{
    DEBUG("WebservMakeResponseEvent::make_response()");
    StatusCode code = result->status_code();
    Response *res = Response::from_success_status_code(
            code,
            result->file()
    );

    std::map<std::string, std::string>::const_iterator ite = result->header().cbegin();
    std::map<std::string, std::string>::const_iterator end = result->header().cend();
    while(ite != end){
        res->add_header(ite->first, ite->second);
        ite++;
    }
    WebservFile *file = res->get_file();
    if(file){
        if(file->is_chunk()){
            MYINFO("Transfer-Encoding: chunked");
            res->add_header(TRANSFER_ENCODING, CHUNKED);
        }else{
            MYINFO("CONTENT_LENGTH: " + Utility::to_string(file->size()));
            res->add_header(CONTENT_LENGTH, Utility::to_string(file->size()));
        }
    }
    res->check_body_and_chunk();
    return (res);
}

Response *WebservMakeResponseEvent::make(WebservEntity *entity)
{
    ApplicationResult *result = entity->app_result();
    DEBUG("WebservMakeResponseEvent::make()");
    Response *res;
    if(result->is_cgi()){
        res = make_response_for_cgi(result, entity);
    }else{
        res = make_response(result);
    }
    return (res);
}

namespace free_func{
    bool make_response(WebservMakeResponseEvent *event, WebservEntity *entity)
    {
        Response *res = event->make(entity);
        entity->set_response(res);
        //entity->set_completed(res);
        return (entity->completed());
    }
}

WebservMakeResponseEvent *WebservMakeResponseEvent::singleton = NULL;
WebservMakeResponseEvent *WebservMakeResponseEvent::get_instance()
{
    if (WebservMakeResponseEvent::singleton == NULL){
        singleton = new WebservMakeResponseEvent();
    }
    return (singleton);
}

WebservEvent *WebservMakeResponseEvent::from_event(WebservEvent *event, WebservFile *src, WebservFile *dst)
{
    DEBUG("WebservMakeResponseEvent::from_event");
    WebservMakeResponseEvent *res_event = WebservMakeResponseEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( res_event, free_func::make_response, event->entity());
    new_event->entity()->io().set_source(src);
    new_event->entity()->io().set_destination(dst);

    return (new_event);
};


WebservEvent* WebservMakeResponseEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservMakeResponseEvent::make_next_event");
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *file = file_factory->make_webserv_file_regular(event->entity()->fd(), event->entity()->response());

    WebservEvent *new_event = (event_factory->make_io_socket_event_as_write(event, file));
    return (new_event);
}

E_EpollEvent WebservMakeResponseEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_WRITE);
}

