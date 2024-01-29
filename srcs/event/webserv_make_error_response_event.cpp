#include "webserv_make_response_event.hpp"
#include "webserv_make_error_response_event.hpp"
#include "webserv_event.hpp"
#include "http_exception.hpp"
#include "opened_socket_file.hpp"
#include "application_result.hpp"
#include "error_file.hpp"

WebservMakeErrorResponseEvent::WebservMakeErrorResponseEvent(
                            FileDiscriptor fd,
                            Request *req
                            //IReader *reader
                            //Config  *cfg
                            )
                            :
                            fd_(fd),
                            req_(req),
                            res_(NULL),
                            file_(NULL),
                            source_file(NULL),
                            destination_file(NULL),
                            timeout_count_(0),
                            is_completed_(false)
                            //reader(reader)
                            //cfg(cfg)
{

};

WebservMakeErrorResponseEvent::~WebservMakeErrorResponseEvent()
{
};


Response* WebservMakeErrorResponseEvent::make_response()
{
    //(void)event;

    cout << "test No.1" << endl;
    printf("src=%p\n", this->src());

    //ApplicationResult *result = dynamic_cast<ApplicationResult*>(this->src());
    //ApplicationResult *result = static_cast<ApplicationResult*>(this->src());
    File *file = ErrorFile::from_status_code(this->code);
    cout << "test No.2" << endl;

    //StatusCode code = result->status_code();
    cout << "test No.3" << endl;

    Response *res = Response::from_success_status_code(
            this->code,
            file
    );

    cout << "test No.4" << endl;
    cout << "test No.2" << endl;

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
    return (res);
}

WebservMakeErrorResponseEvent *WebservMakeErrorResponseEvent::from_event(WebservEvent *event, StatusCode code, File *dst, IWriter *writer)
{
    DEBUG("WebservMakeErrorResponseEvent::from_event");
  //(void)src;
  //(void)dst;
  WebservMakeErrorResponseEvent *new_event = new WebservMakeErrorResponseEvent(event->fd(), event->req());
  //printf("src=%p\n", src);
  new_event->source_file = NULL;
  new_event->destination_file = dst;
  new_event->next_event_writer = writer;
  new_event->code = code;
  new_event->entity_ = event->entity();
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
    File *src = event->res();
    //File *dst = OpenedSocketFile::from_fd(this->next_event_writer, event->fd());
    return (event_factory->make_io_socket_event_as_write(event, src));
}

E_EpollEvent WebservMakeErrorResponseEvent::get_next_epoll_event()
{
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

File *WebservMakeErrorResponseEvent::make()
{
    DEBUG("WebservMakeErrorResponseEvent::make()");
    return (make_response());
}

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


void WebservMakeErrorResponseEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservMakeErrorResponseEvent::cgi_event()
{
    return (this->cgi_event_);
}

void WebservMakeErrorResponseEvent::set_file(File *file)
{
    DEBUG("WebservMakeErrorResponseEvent::set_file");
    this->res_ = static_cast<Response*>(file);
}

Entity *WebservMakeErrorResponseEvent::entity()
{
    return (this->entity_);
}


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

