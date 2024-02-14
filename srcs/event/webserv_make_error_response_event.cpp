#include "webserv_make_response_event.hpp"
#include "webserv_make_error_response_event.hpp"
#include "webserv_event.hpp"
#include "http_exception.hpp"
#include "opened_socket_file.hpp"
#include "application_result.hpp"
#include "error_file.hpp"

WebservMakeErrorResponseEvent::WebservMakeErrorResponseEvent()
{
    ;
}

WebservMakeErrorResponseEvent::~WebservMakeErrorResponseEvent()
{
    ;
}


Response* WebservMakeErrorResponseEvent::make_response(ApplicationResult *result)
{
    (void)result;

    /*
    cout << "test No.1" << endl;
    //printf("src=%p\n", this->src());

    //ApplicationResult *result = dynamic_cast<ApplicationResult*>(this->src());
    //ApplicationResult *result = static_cast<ApplicationResult*>(this->src());
    File *file = ErrorFile::from_status_code(result->status_code());
    cout << "test No.2" << endl;

    //StatusCode code = result->status_code();
    cout << "test No.3" << endl;

    Response *res = Response::from_success_status_code(
            result->status_code(),
            file
    );

    cout << "test No.4" << endl;
    cout << "test No.2" << endl;
    */

    //WebservMakeErrorResponseEvent *new_event = new WebservMakeErrorResponseEvent();
    //write_event->source_file = src;
    //write_event->destination_file = dst;

    //std::map<std::string, std::string>::const_iterator ite = result->header().begin();
    //std::map<std::string, std::string>::const_iterator end = result->header().end();
    cout << "test No.3" << endl;
    /*
    while(ite != end){
        res->add_header(ite->first, ite->second);
        ite++;
    }
    */
    //res->check_body_and_chunk();
    cout << "test No.4" << endl;
    //return (res);
    return (NULL);
}


void make(WebservMakeErrorResponseEvent *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
    //ApplicationResult *result = entity->app_result();
    //Response *res = event->make_response(event->entity()->app_result());
    //entity->set_response(res);
}


WebservEvent *WebservMakeErrorResponseEvent::from_event(WebservEvent *event, StatusCode code, File *dst)
{
    DEBUG("WebservMakeErrorResponseEvent::from_event");
    //(void)src;
    //(void)dst;
    WebservMakeErrorResponseEvent *error_event = new WebservMakeErrorResponseEvent();
    (void)code;
    //error_event->next_event_writer = writer;
    //error_event->code = code;

    WebservEvent *new_event =  new WebservEvent( error_event, make, event->entity());

    new_event->entity()->io().set_source(NULL);
    new_event->entity()->io().set_destination(dst);

    //new_event->entity_ = event->entity();
    return (new_event);
    //return (event);
    //return (new WebservMakeErrorResponseEvent());
};

EWebservEvent WebservMakeErrorResponseEvent::which()
{
    return (MAKE_EVENT);
}

WebservEvent* WebservMakeErrorResponseEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservMakeErrorResponseEvent::make_next_event");
    File *src = event->entity()->response();
    //File *dst = OpenedSocketFile::from_fd(this->next_event_writer, event->fd());
    return (event_factory->make_io_socket_event_as_write(event, src));
}

E_EpollEvent WebservMakeErrorResponseEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_WRITE);
}

bool WebservMakeErrorResponseEvent::check_body_size(Request *req, const ConfigServer *server)
{

    ssize_t max_body_size = (ssize_t)server->get_max_body_size();

    Header const &header = req->header();
    ssize_t content_len = 0;

    try{
        content_len = header.get_content_length();
    }catch(std::invalid_argument &e){
        ERROR("Invalid Request Content-Length:" + header.get_content_length_str());
        throw HttpException("400");
    }

    int cur_body_size = 0;
    req->get_buf_body(&cur_body_size);

    if(content_len < 0 && cur_body_size > 0){
        ERROR("Invalid Request. Content-Length is not set but body is. body size:" + Utility::to_string(max_body_size));
        throw HttpException("411");
    }
    if(content_len > max_body_size){
        ERROR("Invalid Request. Content-Length is more than " + Utility::to_string(max_body_size));
        throw HttpException("413");
    }

    return (true);
}



/*
File *WebservMakeErrorResponseEvent::make()
{
    DEBUG("WebservMakeErrorResponseEvent::make()");
    return (make_response());
}
*/

/*
FileDiscriptor &WebservMakeErrorResponseEvent::fd()
{
    return (this->fd_);
}

Request *WebservMakeErrorResponseEvent::req()
{
    return (req_);
}

Response *WebservMakeErrorResponseEvent::res()
{
    return (this->res_);
}
*/

/*
File *WebservMakeErrorResponseEvent::src()
{
    return (this->source_file);
}

File *WebservMakeErrorResponseEvent::dst()
{
    return (this->destination_file);
}

void WebservMakeErrorResponseEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservMakeErrorResponseEvent::set_dst(File *file)
{
    this->destination_file = file;
}

bool WebservMakeErrorResponseEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservMakeErrorResponseEvent::set_completed(bool flag)
{
    DEBUG("WebservMakeErrorResponseEvent::set_completed");
    this->is_completed_ = flag;
}

void WebservMakeErrorResponseEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservMakeErrorResponseEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservMakeErrorResponseEvent::timeout_count()
{
    return (this->timeout_count_);
}
*/


/*
void WebservMakeErrorResponseEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservMakeErrorResponseEvent::cgi_event()
{
    return (this->cgi_event_);
}
*/

/*
void WebservMakeErrorResponseEvent::set_file(File *file)
{
    DEBUG("WebservMakeErrorResponseEvent::set_file");
    //this->res_ = static_cast<Response*>(file);
    this->entity_->set_response(static_cast<Response*>(file));
}
*/

/*
WebservEntity*WebservMakeErrorResponseEvent::entity()
{
    return (this->entity_);
}
*/


/*
void WebservMaker::parse_res(WebservEvent *event)
{
    DEBUG("WebservMaker::parse_res()");

    io_multi_controller->modify(event->fd(), EPOLLOUT | EPOLLONESHOT);
    Response *res = event->res();
    //printf("res=%p\n", res);
    WebservEvent *next_event = event_factory->make_write_event(event, res);
    this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);
    //this->event_manager->erase_event_waiting_epoll(event->fd());
    delete (event);
}
*/

