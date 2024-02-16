#include "webserv_application_with_cgi_event.hpp"
#include "webserv_event.hpp"
#include "opened_socket_file.hpp"
#include "vector_file.hpp"
#include "application_factory.hpp"

WebservApplicationWithCgiEvent::WebservApplicationWithCgiEvent()
{
    ;
}

WebservApplicationWithCgiEvent::~WebservApplicationWithCgiEvent()
{
    ;
}

WebservApplicationWithCgiEvent *WebservApplicationWithCgiEvent::singleton = NULL;
WebservApplicationWithCgiEvent *WebservApplicationWithCgiEvent::get_instance()
{
    if (WebservApplicationWithCgiEvent::singleton == NULL){
        singleton = new WebservApplicationWithCgiEvent();
    }
    return (singleton);
}

WebservEvent *WebservApplicationWithCgiEvent::from_event(WebservEvent *event)
{
    WebservApplicationWithCgiEvent *app_event =  WebservApplicationWithCgiEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( app_event, invoke<WebservApplicationWithCgiEvent>, event->entity());
    return (new_event);
};

EWebservEvent WebservApplicationWithCgiEvent::which()
{
    return (APPLICATION_EVENT);
}

WebservEvent* WebservApplicationWithCgiEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationWithCgiEvent::make_next_event");

    Request *req = event->entity()->request();
    VectorFile *file = VectorFile::from_ref(req->req_line().uri().query());
    req->set_file(file);
    File *write_src = event->entity()->request();
    File *read_dst = event->entity()->app_result();
    ApplicationResult *result = event->entity()->app_result();
    VectorFile *result_file = VectorFile::from_buf_size(MAX_BUF);
    result->set_file(result_file);
    result->set_is_cgi(true);

    return (event_factory->make_io_socket_for_cgi(event, write_src, read_dst, result));
}

E_EpollEvent WebservApplicationWithCgiEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_FOR_CGI);
}

