#include "webserv_make_request_event.hpp"
#include "webserv_event.hpp"
#include "http_exception.hpp"
#include "opened_socket_file.hpp"

WebservMakeRequestEvent::WebservMakeRequestEvent(
                            FileDiscriptor fd,
                            Request *req,
                            IReader *reader,
                            Config  *cfg
                            )
                            :
                            fd_(fd),
                            req_(req),
                            res_(NULL),
                            file_(NULL),
                            source_file(NULL),
                            destination_file(NULL),
                            timeout_count_(0),
                            is_completed_(false),
                            reader(reader),
                            cfg(cfg)
{

};

WebservMakeRequestEvent::~WebservMakeRequestEvent()
{
};

WebservMakeRequestEvent *WebservMakeRequestEvent::from_event(WebservEvent *event, IReader *reader, Config *cfg)
{
    WebservMakeRequestEvent *new_event = new WebservMakeRequestEvent(event->fd(), event->req(), reader, cfg);
    new_event->source_file = event->dst();
    new_event->destination_file = NULL;
    return (new_event);
};

EWebservEvent WebservMakeRequestEvent::which()
{
    return (MAKE_EVENT);
}

WebservEvent* WebservMakeRequestEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    DEBUG("WebservMakeRequestEvent::make_next_event");
    return (event_factory->make_application_event(event));
}

E_EpollEvent WebservMakeRequestEvent::get_next_epoll_event()
{
    return (EPOLL_NONE);
}

bool WebservMakeRequestEvent::check_body_size(Request *req, const ConfigServer *server)
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

File *WebservMakeRequestEvent::make_request()
{
    DEBUG("WebservMakeRequestEvent::parse_req()");
    Request *req = Request::from_fd(this->fd());
    cout << "test No.1" << endl;
    //this->source_file = OpenedSocketFile::from_fd(this->reader, this->fd());

    //char request_buf[MAX_BUF];
    char *buf_p;

    printf("buf_=%p\n", buf_p);
    File *src = this->src();
    size_t buf_size = src->read(&buf_p, MAX_STATUS_LINE);
    (void)buf_size;
    printf("buf_size=%zu\n",buf_size);
    printf("buf_=%p\n", buf_p);
    cout << "test No.2" << endl;

    Split sp(buf_p, "\r\n");
    cout << "test No.3" << endl;
    if(sp.size() == 0){
        //delete (event);
        ERROR("Invalid Request. Reques doesn't have \"\r\n\"");
        throw HttpException("400");
    }
    cout << "test No.4" << endl;
    try{
    cout << "test No.5" << endl;
        req->set_request_line(sp[0]);
    cout << "test No.6" << endl;
        req->set_header(sp, 1);
    cout << "test No.7" << endl;
    }catch (std::invalid_argument &e){
        ERROR("Invalid Request:" + string(e.what()));
        throw HttpException("400");
    }

    cout << "test No.8" << endl;
    req->print_info();
    cout << "test No.9" << endl;
    this->set_completed(true);

    const ConfigServer *server = this->cfg->get_server(req);
    const ConfigLocation *location= this->cfg->get_location(server, req);
    req->set_requested_filepath(location);

    this->check_body_size(req, server);
    return (req);
}


File *WebservMakeRequestEvent::make()
{
    return (make_request());
}

FileDiscriptor WebservMakeRequestEvent::fd()
{
    return (this->fd_);
}

Request *WebservMakeRequestEvent::req()
{
    return (req_);
}

Response *WebservMakeRequestEvent::res()
{
    return (this->res_);
}

File *WebservMakeRequestEvent::src()
{
    return (this->source_file);
}

File *WebservMakeRequestEvent::dst()
{
    return (this->destination_file);
}

void WebservMakeRequestEvent::set_src(File *file)
{
    this->source_file = file;
}

void WebservMakeRequestEvent::set_dst(File *file)
{
    this->destination_file = file;
}

bool WebservMakeRequestEvent::is_completed()
{
    return (this->is_completed_);
}
void WebservMakeRequestEvent::set_completed(bool flag)
{
    this->is_completed_ = flag;
}

void WebservMakeRequestEvent::increase_timeout_count(int count)
{
    this->timeout_count_ += count;
    DEBUG("WebservMakeRequestEvent::increase_timeout_count add:" + Utility::to_string(count) + ", after:" + Utility::to_string(this->timeout_count_));
}

int WebservMakeRequestEvent::timeout_count()
{
    return (this->timeout_count_);
}

/*
void WebservMakeRequestEvent::set_response(Response *res)
{
    this->res_ = res;
}


void WebservMakeRequestEvent::set_file(File *file)
{
    this->file_ = file;
}

File *WebservMakeRequestEvent::file()
{
    return (this->file_);
}
*/

void WebservMakeRequestEvent::set_cgi_event(WebservCgiEvent *cgi_event)
{
    this->cgi_event_ = cgi_event;
}

WebservCgiEvent *WebservMakeRequestEvent::cgi_event()
{
    return (this->cgi_event_);
}

void WebservMakeRequestEvent::set_file(File *file)
{
    this->req_ = static_cast<Request*>(file);
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

