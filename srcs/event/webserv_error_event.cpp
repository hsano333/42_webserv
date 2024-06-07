
#include "webserv_error_event.hpp"
#include "global.hpp"
#include "response.hpp"
#include "request.hpp"
#include "webserv_entity.hpp"
#include "webserv_file_factory.hpp"
#include "socket_writer.hpp"

WebservErrorEvent::WebservErrorEvent()
{
    ;
}

WebservErrorEvent::~WebservErrorEvent()
{
    ;
}


WebservErrorEvent *WebservErrorEvent::singleton = NULL;
WebservErrorEvent *WebservErrorEvent::get_instance()
{
    if (WebservErrorEvent::singleton == NULL){
        singleton = new WebservErrorEvent();
    }
    return (singleton);
}

void WebservErrorEvent::delete_myself()
{
    if(WebservErrorEvent::singleton)
        delete WebservErrorEvent::singleton;
}

namespace free_func{
    bool handle_error(WebservErrorEvent *event, WebservEntity *entity)
    {
        (void)event;
        StatusCode &code = entity->error_code();
        DEBUG("WebservErrorEvent handle_error() code:" + code.to_string());


        Response *res = NULL;

        const Config *cfg = entity->config();
        Request const *req = entity->request();
        if(req){
            ConfigServer const *server = entity->config()->get_server(req);
            ConfigLocation const *location = cfg->get_location(server, req);
            ConfigLimit const *limit = location->limit();


            res = Response::from_error_status_code(entity->fd(), code, location);
            event->check_completed(entity);

            if(code.to_int() == 405){
                std::string allowed_methods = limit->allowed_method_str();
                res->add_header(ALLOW, allowed_methods);
            }else if(code.to_int() == 401){
                res->add_header(WWW_AUTHENTICATE, AUTHENTICATE_BASIC);
            }else if(code.to_int() >= 300 && code.to_int() < 400){
                if(location->is_redirect()){
                    res->add_header(LOCATION, location->redirect().second);
                }else{
                    code = StatusCode::from_int(500);
                }
            }

        }else{
            res = Response::from_error_status_code(entity->fd(), code, NULL);
            event->check_completed(entity);
            if(code.to_int() == 401){
                res->add_header(WWW_AUTHENTICATE, AUTHENTICATE_BASIC);
            }
        }

        entity->set_response(res);
        return (true);
    }
}

WebservEvent *WebservErrorEvent::from_event(WebservEvent *event, StatusCode &code)
{
    DEBUG("WebservErrorEvent::from_event");
    WebservErrorEvent *error_event = WebservErrorEvent::get_instance();
    event->entity()->set_error_code(code);
    return (new WebservEvent( error_event, free_func::handle_error, event->entity(), TIMEOUT_EVENT));
}

WebservEvent* WebservErrorEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservErrorEvent::make_next_event");
    (void)event_factory;
    (void)event;

    Response *res = event->entity()->response();
    WebservFileFactory *file_factory =  WebservFileFactory::get_instance();
    WebservFile *file = file_factory->make_webserv_file_regular(event->entity()->fd(), res);
    return (event_factory->make_io_socket_event_as_write(event, file));
}

E_EpollEvent WebservErrorEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_WRITE);
}

void WebservErrorEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
