
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
        Response const *current_res = entity->response();
        DEBUG("WebservErrorEvent handle_error() delete response address:" + Utility::to_string(current_res));
        StatusCode &code = entity->error_code();

        Response *res = Response::from_error_status_code(entity->fd(), code);
        event->check_completed(entity);

        if(code.to_int() == 405){
            const Config *cfg = entity->config();
            Request const *req = entity->request();
            ConfigServer const *server = entity->config()->get_server(req);
            ConfigLocation const *location = cfg->get_location(server, req);
            ConfigLimit const *limit = location->limit();
            std::string allowed_methods = limit->allowed_method_str();

            res->add_header(ALLOW, allowed_methods);
        }
        entity->set_response(res);

        return (true);
        //if(current_res){
            //current_res->set_satus_code(code);
            //delete current_res;
            //entity->set_response(NULL);
        //}

        WebservFileFactory *file_factory =  WebservFileFactory::get_instance();
        WebservFile *file = file_factory->make_error_file(entity->fd(), code);
        /*
        if(entity->app_result() != NULL){
            delete entity->app_result();
            entity->set_result(NULL);
        }
        */
        ApplicationResult *result = ApplicationResult::from_status_code(code, "NONE");
        result->set_file(file);
        entity->set_result(result);

        return (true);

    }
}

WebservEvent *WebservErrorEvent::from_event(WebservEvent *event, StatusCode &code)
{
    DEBUG("WebservErrorEvent::from_event");
    //Response const *current_res = event->entity()->response();
    WebservErrorEvent *error_event = WebservErrorEvent::get_instance();
    event->entity()->set_error_code(code);
    //WebservIOSocketEvent *io_event = WebservIOSocketEvent::get_instance();

    return (new WebservEvent( error_event, free_func::handle_error, event->entity()));

    //return (new_event);
}

WebservEvent* WebservErrorEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservErrorEvent::make_next_event");
    (void)event_factory;
    (void)event;

    //SocketWriter *socket_writer = SocketWriter::get_instance();
    WebservFileFactory *file_factory =  WebservFileFactory::get_instance();
    //WebservFile *dst = file_factory->make_socket_file(event->entity()->fd(), event->entity()->fd(), socket_writer, NULL);
    //WebservFile *result_file = file_factory->make_webserv_file_regular(event->entity()->fd(), event->entity()->app_result());

    WebservFile *file = file_factory->make_webserv_file_regular(event->entity()->fd(), event->entity()->response());
    return (event_factory->make_io_socket_event_as_write(event, file));
    //return (event_factory->make_making_response_event(event, result_file, dst));
    //return (WebservMakeResponseEvent::from_event(event, result_file, dst));
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
