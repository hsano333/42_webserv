#include "webserv_make_response_event.hpp"
#include "webserv_event.hpp"
#include "webserv_io_event.hpp"
#include "http_exception.hpp"
#include "opened_socket_file.hpp"
#include "application_result.hpp"
#include "header_word.hpp"

WebservMakeResponseEvent::WebservMakeResponseEvent():
                            file_(NULL),
                            source_file(NULL),
                            destination_file(NULL),
                            timeout_count_(0),
                            is_completed_(false),
                            reader(NULL)
                            //reader(reader)
                            //cfg(cfg)
{

};

WebservMakeResponseEvent::~WebservMakeResponseEvent()
{
};


Response* WebservMakeResponseEvent::make_response_for_cgi(ApplicationResult *result)
{
    DEBUG("WebservMakeResponseEvent::make_response_for_cgi()");

    char *data;
    int read_size = result->file()->read(&data, MAX_READ_SIZE);
    DEBUG("WebservMakeResponseEvent::make_response_for_cgi() No.2");

    if(read_size <= 0){
        ERROR("CGI Respons Data is nothing or error");
        throw HttpException("500");
    }
    DEBUG("WebservMakeResponseEvent::make_response_for_cgi() No.3");

    // RFC3875にはNL(New Line)の具体的な定義がないため、
    // 改行文字はLFとする(Linuxでの運用を想定しているため)
    char *body_p = Utility::strnstr(data, LF2, read_size);
    DEBUG("WebservMakeResponseEvent::make_response_for_cgi() No.4");
    if(!body_p){
        ERROR("CGI Statement Error: not find LFLF(double New Line)");
        throw HttpException("500");
    }
    cout << "body_p:" << body_p << endl;
    cout << "data:" << data << endl;
    printf("body_p=%p\n", body_p);
    printf("data=%p\n", data);

    size_t header_size = body_p - data;
    if(header_size >= MAX_STATUS_LINE){
        ERROR("exceed CGI Response Status Line" + Utility::to_string(header_size));
        throw HttpException("exceed CGI Response Status Line:" + Utility::to_string(header_size));
    }
    *body_p = '\0';

    DEBUG("WebservMakeResponseEvent::make_response_for_cgi() No.5");
    Split headers_line(data, LF, false, true);
    File *file = OpenedSocketFile::from_fd(reader, result->cgi_out());

    DEBUG("WebservMakeResponseEvent::make_response_for_cgi() No.6");
    Request const *req = this->entity_->request();
    ConfigServer const *server = this->entity_->config()->get_server(req);
    DEBUG("WebservMakeResponseEvent::make_response_for_cgi() No.7");
    //const ConfigLocation *location = this->entity()->cfg->get_location(server, req);

    Response *res = Response::from_cgi_header_line(headers_line, file);

    int tmp_size = read_size - header_size - 4;
    res->set_buf_body(body_p + 4, tmp_size);
    //res->buf_body = body_p + 4;


    printf("res=%p\n", res);
    printf("req=%p\n", req);
    printf("server=%p\n", server);
    DEBUG("WebservMakeResponseEvent::make_response_for_cgi() No.8");
    if(!res->check_body_size(server))
    {
        delete res;
        throw HttpException("500");
    }
    DEBUG("WebservMakeResponseEvent::make_response_for_cgi() No.9");

    //req->set_requested_filepath(location);
    //this->check_body_size(req, server);
    //req->set_cgi(check_cgi(req, location));

    return (res);

    /*
    //Response *res = 
    printf("heder print printf()\n");
    headers_line.print();
    for(size_t i=0;i<headers_line.size();i++){
        DEBUG(headers_line[i]);
    }

    //string status_code = headers_lind.find("");
    sleep(5);
    std::exit(0);

    DEBUG("cgi response=" + string(data));
    cout << "cgi read_size=" << read_size << endl;


    StatusCode code = result->status_code();
    Response *res = Response::from_success_status_code(
            code,
            result->file()
    );
    return (res);
    */
}

Response* WebservMakeResponseEvent::make_response(ApplicationResult *result)
{
    DEBUG("WebservMakeResponseEvent::make_response()");
    //(void)event;

    cout << "test No.1" << endl;
    printf("src=%p\n", this->src());

    //ApplicationResult *result = dynamic_cast<ApplicationResult*>(this->src());
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

    std::map<std::string, std::string>::const_iterator ite = result->header().cbegin();
    std::map<std::string, std::string>::const_iterator end = result->header().cend();
    cout << "test No.3" << endl;
    while(ite != end){
        res->add_header(ite->first, ite->second);
        ite++;
    }
    res->check_body_and_chunk();
    cout << "test No.4" << endl;
    return (res);
}

Response *WebservMakeResponseEvent::make(ApplicationResult *result)
{
    DEBUG("WebservMakeResponseEvent::make()");
    if(result->is_cgi()){
        return (make_response_for_cgi(result));
    }
    return (make_response(result));
}

namespace FREE
{
    void make(WebservMakeResponseEvent *event, WebservEntity *entity)
    {
        ApplicationResult *result = entity->app_result();
        Response *res = event->make(result);
        entity->set_response(res);
    }
}


WebservEvent *WebservMakeResponseEvent::from_event(WebservEvent *event, File *src, File *dst, IReader *reader)
{
    DEBUG("WebservMakeResponseEvent::from_event");
    //(void)src;
    //(void)dst;
    WebservMakeResponseEvent *res_event = new WebservMakeResponseEvent();
    WebservEvent *new_event =  new WebservEvent( res_event, FREE::make, event->entity());
    //new_event->entity_ = event->entity();
    new_event->entity()->io()->set_source(src);
    new_event->entity()->io()->set_destination(dst);


    //todo
    //res_event->next_event_writer = writer; //socket_writer
    res_event->reader = reader;            //normal_reader

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
    WebservEvent *new_event = (event_factory->make_io_socket_event_as_write(event, this->entity_->response()));
    //this->res_->set_file()();
    //WebservIOEvent *io_event = dynamic_cast<WebservIOEvent*>(new_event);
    //File *socket_io = OpenedSocketFile::from_fd(fd, socket_writer, socket_reader);

    //File *src = new_event->dst();
    //File *dst = OpenedSocketFile::from_fd(this->next_event_writer, event->fd());


    //io_event->set_write_io(src, dst);
    return (new_event);
}

E_EpollEvent WebservMakeResponseEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_WRITE);
}

/*
bool WebservMakeResponseEvent::check_body_size(Request *req, const ConfigServer *server)
{
    DEBUG("WebservMakeResponseEvent::check_body_size");

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
*/


/*
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
*/

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
    //this->res_ = static_cast<Response*>(file);
    this->entity_->set_response(static_cast<Response*>(file));
}

WebservEntity*WebservMakeResponseEvent::entity()
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

