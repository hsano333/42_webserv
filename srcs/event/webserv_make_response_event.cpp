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


Response* WebservMakeResponseEvent::make_response(ApplicationResult *result, WebservEntity *entity)
{
    DEBUG("WebservMakeResponseEvent::make_response()");
    StatusCode code = result->status_code();
    const Config *cfg = entity->config();
    Request const *req = entity->request();
    ConfigServer const *server = entity->config()->get_server(req);
    ConfigLocation const *location = cfg->get_location(server, req);
    Response *res = Response::from_success_status_code(
            entity->fd(),
            code,
            result->file(),
            location
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
    if(!res->check_body_and_chunk()){
        DEBUG("WebservMakeResponseEvent::make_response() ");
    }

    if(code.to_int() == 405){
        const Config *cfg = entity->config();
        Request const *req = entity->request();
        ConfigServer const *server = entity->config()->get_server(req);
        ConfigLocation const *location = cfg->get_location(server, req);
        ConfigLimit const *limit = location->limit();
        std::string allowed_methods = limit->allowed_method_str();

        res->add_header(ALLOW, allowed_methods);
    }

    return (res);
}

Response *WebservMakeResponseEvent::make(WebservEntity *entity)
{
    ApplicationResult *result = entity->app_result();
    DEBUG("WebservMakeResponseEvent::make()");
    Response *res;
    res = make_response(result, entity);
    return (res);
}

namespace free_func{
    bool make_response(WebservMakeResponseEvent *event, WebservEntity *entity)
    {
        Response *res = event->make(entity);
        entity->set_response(res);
        entity->set_completed(true);
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

void WebservMakeResponseEvent::delete_myself()
{
    if(WebservMakeResponseEvent::singleton)
        delete WebservMakeResponseEvent::singleton;
}

WebservEvent *WebservMakeResponseEvent::from_event(WebservEvent *event, WebservFile *src, WebservFile *dst)
{
    DEBUG("WebservMakeResponseEvent::from_event");
    WebservMakeResponseEvent *res_event = WebservMakeResponseEvent::get_instance();
    WebservEvent *new_event = new WebservEvent( res_event, free_func::make_response, event->entity());
    new_event->entity()->io().set_source(src);
    new_event->entity()->io().set_destination(dst);

    return (new_event);
};


WebservEvent* WebservMakeResponseEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservMakeResponseEvent::make_next_event");
    Response *res = event->entity()->response();
    WebservFileFactory *file_factory = WebservFileFactory::get_instance();
    WebservFile *file = file_factory->make_webserv_file_regular(event->entity()->fd(), res);

    WebservEvent *new_event = (event_factory->make_io_socket_event_as_write(event, file ));
    return (new_event);
}

E_EpollEvent WebservMakeResponseEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_WRITE);
}

void WebservMakeResponseEvent::check_completed(WebservEntity * entity)
{
    DEBUG("WebservMakeResponseEvent::check_completed");
    entity->set_completed(true);
}
