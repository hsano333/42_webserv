#include "webserv_make_response_event.hpp"
#include "webserv_event.hpp"
#include "webserv_io_event.hpp"
#include "http_exception.hpp"
#include "opened_socket_file.hpp"
#include "application_result.hpp"

WebservMakeResponseEvent::WebservMakeResponseEvent(
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

WebservMakeResponseEvent::~WebservMakeResponseEvent()
{
};


Response* WebservMakeResponseEvent::make_response()
{
    DEBUG("WebservMakeResponseEvent::make_response()");
    //(void)event;

    cout << "test No.1" << endl;
    printf("src=%p\n", this->src());

    //ApplicationResult *result = dynamic_cast<ApplicationResult*>(this->src());
    ApplicationResult *result = static_cast<ApplicationResult*>(this->src());
    StatusCode code = result->status_code();
    Response *res = Response::from_success_status_code(
            code,
            result->file()
    );

    cout << "test No.4" << endl;
    cout << "test No.2" << endl;

    //WebservMakeResponseEvent *new_event = new WebservMakeResponseEvent();
    //write_event->source_file = src;
    //write_event->destination_file = dst;

    std::map<std::string, std::string>::const_iterator ite = result->header().begin();
    std::map<std::string, std::string>::const_iterator end = result->header().end();
    cout << "test No.3" << endl;
    while(ite != end){
        res->add_header(ite->first, ite->second);
        ite++;
    }
    res->check_body_and_chunk();
    cout << "test No.4" << endl;
    return (res);
}

WebservMakeResponseEvent *WebservMakeResponseEvent::from_event(WebservEvent *event, File *src, File *dst, IWriter *writer)
{
    DEBUG("WebservMakeResponseEvent::from_event");
  //(void)src;
  //(void)dst;
  WebservMakeResponseEvent *new_event = new WebservMakeResponseEvent(event->fd(), event->req());
  printf("src=%p\n", src);
  new_event->source_file = src;
  new_event->destination_file = dst;
  new_event->next_event_writer = writer;
  return (new_event);
  //return (event);
  //return (new WebservMakeResponseEvent());
};

EWebservEvent WebservMakeResponseEvent::which()
{
    return (MAKE_EVENT);
}

WebservEvent* WebservMakeResponseEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservMakeResponseEvent::make_next_event");
    WebservEvent *new_event = (event_factory->make_io_socket_event_as_write(event, this->res_));
    //WebservIOEvent *io_event = dynamic_cast<WebservIOEvent*>(new_event);
    //File *socket_io = OpenedSocketFile::from_fd(fd, socket_writer, socket_reader);

    //File *src = new_event->dst();
    //File *dst = OpenedSocketFile::from_fd(this->next_event_writer, event->fd());


    //io_event->set_write_io(src, dst);
    return (new_event);
}

E_EpollEvent WebservMakeResponseEvent::get_next_epoll_event()
{
    return (EPOLL_WRITE);
}

bool WebservMakeResponseEvent::check_body_size(Request *req, const ConfigServer *server)
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

File *WebservMakeResponseEvent::make()
{
    DEBUG("WebservMakeResponseEvent::make()");
    return (make_response());
}

FileDiscriptor &WebservMakeResponseEvent::fd()
{
    return (this->fd_);
}

Request *WebservMakeResponseEvent::req()
{
    return (req_);
}

Response *WebservMakeResponseEvent::res()
{
    return (this->res_);
}

File *WebservMakeResponseEvent::src()
{
    return (this->source_file);
}

File *WebservMakeResponseEvent::dst()
{
    return (this->destination_file);
}

void WebservMakeResponseEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservMakeResponseEvent::set_dst(File *file)
{
    this->destination_file = file;
}

bool WebservMakeResponseEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservMakeResponseEvent::set_completed(bool flag)
{
    DEBUG("WebservMakeResponseEvent::set_completed");
    this->is_completed_ = flag;
}

void WebservMakeResponseEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservMakeResponseEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservMakeResponseEvent::timeout_count()
{
    return (this->timeout_count_);
}


void WebservMakeResponseEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservMakeResponseEvent::cgi_event()
{
    return (this->cgi_event_);
}

void WebservMakeResponseEvent::set_file(File *file)
{
    DEBUG("WebservMakeResponseEvent::set_file");
    this->res_ = static_cast<Response*>(file);
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

