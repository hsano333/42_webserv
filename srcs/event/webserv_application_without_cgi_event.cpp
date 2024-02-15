#include "webserv_application_without_cgi_event.hpp"
#include "webserv_event.hpp"

WebservApplicationWithoutCgiEvent::WebservApplicationWithoutCgiEvent()
{
    ;
}

WebservApplicationWithoutCgiEvent::~WebservApplicationWithoutCgiEvent()
{
    ;
}

void execute(WebservApplicationWithoutCgiEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
    //Request *req = event->make_request(entity);
    //event->entity()->set_request(req);
}

WebservApplicationWithoutCgiEvent *WebservApplicationWithoutCgiEvent::singleton = NULL;
WebservApplicationWithoutCgiEvent *WebservApplicationWithoutCgiEvent::get_instance()
{
    if (WebservApplicationWithoutCgiEvent::singleton == NULL){
        singleton = new WebservApplicationWithoutCgiEvent();
    }
    return (singleton);
}

#include "webserv_executer.hpp"
WebservEvent *WebservApplicationWithoutCgiEvent::from_event(WebservEvent *event)
{
    WebservApplicationWithoutCgiEvent *app_event = WebservApplicationWithoutCgiEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( app_event, invoke<WebservApplicationWithoutCgiEvent>, event->entity());
    //new_event->entity_ = event->entity();
    return (new_event);
};

EWebservEvent WebservApplicationWithoutCgiEvent::which()
{
    return (APPLICATION_EVENT);
}

WebservEvent* WebservApplicationWithoutCgiEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservApplicationWithoutCgiEvent::make_next_event");
    (void)event_factory;
    //todo
    /*
    if(event->cgi_event()){
        cout << "test No.1 " << endl;
        WebservCgiEvent *cgi = event->cgi_event();
        cout << "test No.2 cgi=" << cgi << endl;
        cout << "test No.2 event->req()=" << event->req() << endl;
        //event_factory->make_and_push_write_cgi_event(cgi->pid(), cgi->fd_out(), event->req());
        event_factory->make_and_push_read_cgi_event(cgi->pid(), cgi->fd_in());
        MYINFO("cgi pid=" + Utility::to_string(cgi->pid()));
        MYINFO("cgi fd_in=" + cgi->fd_in().to_string());
        MYINFO("cgi fd_out=" + cgi->fd_out().to_string());
        cout << "test No.20" << endl;

        return (event_factory->make_nothing_event(cgi->pid(), cgi->pid()));
        //return (event_factory->make_write_event(event, event->res()));
    }
    */
    //return (event_factory->make_write_event(event, event->res()));
    //printf("result=%p\n", this->result());
    //cout << "status code=" << this->result()->status_code().to_string() << endl;
    return (event_factory->make_making_response_event(event, event->entity()->io().destination()));
    //return (event);
}

E_EpollEvent WebservApplicationWithoutCgiEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
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

/*
FileDiscriptor &WebservApplicationWithoutCgiEvent::fd()
{
    return (this->fd_);
}

Request *WebservApplicationWithoutCgiEvent::req()
{
    return (req_);
}

Response *WebservApplicationWithoutCgiEvent::res()
{
    return (this->res_);
}

File *WebservApplicationWithoutCgiEvent::src()
{
    return (this->source_file);
}

File *WebservApplicationWithoutCgiEvent::dst()
{
    return (this->destination_file);
}

void WebservApplicationWithoutCgiEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservApplicationWithoutCgiEvent::set_dst(File *file)
{
    this->destination_file = file;
}


bool WebservApplicationWithoutCgiEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservApplicationWithoutCgiEvent::set_completed(bool flag)
{
    DEBUG("WebservApplicationWithoutCgiEvent::set_completed");
    this->is_completed_ = flag;
}

void WebservApplicationWithoutCgiEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservApplicationWithoutCgiEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservApplicationWithoutCgiEvent::timeout_count()
{
    return (this->timeout_count_);
}

void WebservApplicationWithoutCgiEvent::set_response(Response *res)
{
    this->res_ = res;
}


void WebservApplicationWithoutCgiEvent::set_file(File *file)
{
    this->file_ = file;
}

File *WebservApplicationWithoutCgiEvent::file()
{
    return (this->file_);
}

void WebservApplicationWithoutCgiEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    DEBUG("WebservApplicationWithoutCgiEvent::set_cgi_event()");
    cout << "copy cgi_event=" << cgi_event << endl;
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservApplicationWithoutCgiEvent::cgi_event()
{
    DEBUG("WebservApplicationWithoutCgiEvent::cgi_event()");
    return (this->cgi_event_);
}

ApplicationResult *WebservApplicationWithoutCgiEvent::result()
{
    return (this->result_);
}

void WebservApplicationWithoutCgiEvent::set_result(ApplicationResult *result)
{
    this->result_ = result;
}

WebservEntity*WebservApplicationWithoutCgiEvent::entity()
{
    return (this->entity_);
}
*/
