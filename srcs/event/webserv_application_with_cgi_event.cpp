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
    //Request *req = event->make_request(entity);
    //event->entity()->set_request(req);
}

WebservEvent *WebservApplicationWithCgiEvent::from_event(WebservEvent *event)
{
    //new_event->next_event_writer = writer;
    WebservApplicationWithCgiEvent *app_event =  new WebservApplicationWithCgiEvent();
    WebservEvent *new_event =  new WebservEvent( app_event, execute, event->entity());
    //new_event->next_event_writer = writer;
    //new_event->entity_ = event->entity();
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
    File *read_dst = event->entity()->io()->destination();
    ApplicationResult *result = static_cast<ApplicationResult*>(event->entity()->io()->destination());
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

/*
FileDiscriptor const &WebservApplicationWithCgiEvent::fd()
{
    return (this->entity()->fd());
}

Request *WebservApplicationWithCgiEvent::req()
{
    return (req_);
}

Response *WebservApplicationWithCgiEvent::res()
{
    return (this->res_);
}
*/

/*
File *WebservApplicationWithCgiEvent::src()
{
    return (this->source_file);
}

File *WebservApplicationWithCgiEvent::dst()
{
    return (this->destination_file);
}

void WebservApplicationWithCgiEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservApplicationWithCgiEvent::set_dst(File *file)
{
    this->destination_file = file;
    //this->destination_file = OpenedSocketFile::from_fd(this->next_event_writer, file->cgi_in())
}


bool WebservApplicationWithCgiEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservApplicationWithCgiEvent::set_completed(bool flag)
{
    DEBUG("WebservApplicationWithCgiEvent::set_completed");
    this->is_completed_ = flag;
}

void WebservApplicationWithCgiEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservApplicationWithCgiEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservApplicationWithCgiEvent::timeout_count()
{
    return (this->timeout_count_);
}


void WebservApplicationWithCgiEvent::set_file(File *file)
{
    this->file_ = file;
}

File *WebservApplicationWithCgiEvent::file()
{
    return (this->file_);
}

void WebservApplicationWithCgiEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    DEBUG("WebservApplicationWithCgiEvent::set_cgi_event()");
    cout << "copy cgi_event=" << cgi_event << endl;
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservApplicationWithCgiEvent::cgi_event()
{
    DEBUG("WebservApplicationWithCgiEvent::cgi_event()");
    return (this->cgi_event_);
}
*/


/*
ApplicationResult *WebservApplicationWithCgiEvent::result()
{
    return (this->result_);
}
*/


/*
void WebservApplicationWithCgiEvent::set_response(Response *res)
{
    this->entity_->set_response(res);
}

void WebservApplicationWithCgiEvent::set_result(ApplicationResult *result)
{
    this->entity_->set_result(result);
}

WebservEntity *WebservApplicationWithCgiEvent::entity()
{
    return (this->entity_);
}
*/
