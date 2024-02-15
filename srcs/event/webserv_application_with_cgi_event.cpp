#include "webserv_application_with_cgi_event.hpp"
#include "webserv_event.hpp"
#include "opened_socket_file.hpp"
#include "vector_file.hpp"

WebservApplicationWithCgiEvent::WebservApplicationWithCgiEvent()
{
    ;
}

WebservApplicationWithCgiEvent::~WebservApplicationWithCgiEvent()
{
    ;
}

void execute(WebservApplicationWithCgiEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
}

WebservApplicationWithCgiEvent *WebservApplicationWithCgiEvent::singleton = NULL;
WebservApplicationWithCgiEvent *WebservApplicationWithCgiEvent::get_instance()
{
    if (WebservApplicationWithCgiEvent::singleton == NULL){
        singleton = new WebservApplicationWithCgiEvent();
    }
    return (singleton);
}

#include "application_factory.hpp"
/*
template<typename EventT>
void invoke(EventT event, WebservEntity *entity)
{
    DEBUG("WebservExecuter::execute");
    ApplicationFactory *factory = ApplicationFactory::get_instance();
    Application *app = factory->make_application(entity);

    bool is_completed = app->invoke(entity);
    entity->set_completed(is_completed);
    ApplicationResult *result = app->get_result();

    entity->set_result(result);
    delete app;
}
*/

#include "webserv_executer.hpp"
WebservEvent *WebservApplicationWithCgiEvent::from_event(WebservEvent *event)
{
    WebservApplicationWithCgiEvent *app_event =  WebservApplicationWithCgiEvent::get_instance();
    //template void invoke(WebservApplicationWithCgiEvent *event, WebservEntity *entity) invoke_func;
    //typedef invoke<WebservApplicationWithCgiEvent> invoke_func;
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
    MYINFO("Query=" + req->req_line().uri().query());
    VectorFile *file = VectorFile::from_ref(req->req_line().uri().query());
    req->set_file(file);
    //file->write();
    File *write_src = event->entity()->request();
    File *read_dst = event->entity()->app_result();
    printf("write_src=%p\n", write_src);
    printf("read_src=%p\n", write_src);
    //ApplicationResult *result = static_cast<ApplicationResult*>(event->entity()->io().destination());
    ApplicationResult *result = event->entity()->app_result();
    VectorFile *result_file = VectorFile::from_buf_size(MAX_BUF);
    result->set_file(result_file);
    result->set_is_cgi(true);
    //File *write_dst = OpenedSocketFile::from_fd(this->next_event_writer, result->cgi_in());
    DEBUG("cgi_in=" + Utility::to_string(result->cgi_in()));

    return (event_factory->make_io_socket_for_cgi(event, write_src, read_dst, result));
}

E_EpollEvent WebservApplicationWithCgiEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_FOR_CGI);
    /*
    if (this->is_completed_){
        if(this->cgi_event() == NULL){
            return (EPOLL_WRITE);
        }else{
            return (EPOLL_CGI_STOP);
        }
    }else{
        return (EPOLL_READ);
    }
    */
}

