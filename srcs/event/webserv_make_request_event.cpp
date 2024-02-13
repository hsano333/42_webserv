#include "webserv_make_request_event.hpp"
#include "webserv_event.hpp"
#include "http_exception.hpp"
#include "opened_socket_file.hpp"

WebservMakeRequestEvent::WebservMakeRequestEvent()
{
    ;
}

WebservMakeRequestEvent::~WebservMakeRequestEvent()
{
};


/*
File *WebservMakeRequestEvent::make()
{
    Request *req = (make_request());
    this->entity()->set_request(req);
    return (req);
}
*/

void make(WebservMakeRequestEvent *event, WebservEntity *entity)
{
    Request *req = event->make_request(entity);
    event->entity()->set_request(req);
}

WebservEvent *WebservMakeRequestEvent::from_event(WebservEvent *event, File *src, File *dst)
{
    DEBUG("WebservMakeRequestEvent::from_event");
    WebservMakeRequestEvent *req_event = new WebservMakeRequestEvent();
    WebservEvent *new_event =  new WebservEvent( req_event, make, event->entity());
    //new_event->entity_ = event->entity();
    new_event->entity()->io()->set_source(src);
    new_event->entity()->io()->set_destination(dst);

    //new_event->reader = reader; //socket_reader

    return (new_event);
};

EWebservEvent WebservMakeRequestEvent::which()
{
    return (MAKE_EVENT);
}

WebservEvent* WebservMakeRequestEvent::make_next_event(WebservEvent* event, WebservEventFactory *event_factory)
{
    return (event_factory->make_application_event(event));
}

E_EpollEvent WebservMakeRequestEvent::get_next_epoll_event(WebservEvent *event)
{
    (void)event;
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
        ERROR("Invalid Request. Content-Length is not set but body exist. body size:" + Utility::to_string(max_body_size));
        throw HttpException("411");
    }
    if(content_len > max_body_size){
        ERROR("Invalid Request. Content-Length is more than " + Utility::to_string(max_body_size));
        throw HttpException("413");
    }

    return (true);
}


void WebservMakeRequestEvent::parse_request(Request *req)
{
    DEBUG("WebservMakeRequestEvent::parse_request");

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

    Split sp(buf_p, CRLF);
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

    //cout << "test No.8" << endl;
    //req->print_info();
    //cout << "test No.9" << endl;
    //this->set_completed(true);

    //return (req);

}

bool WebservMakeRequestEvent::check_cgi(const Request *req, const ConfigLocation *location) const
{
    DEBUG("WebservMakeRequestEvent::check_cgi");
    (void)req;
    (void)location;
    const ConfigLimit *limit = location->limit();

    if (req->is_file() == false){
        WARNING("not CGI");
        return (false);
        //throw std::invalid_argument("not CGI");
    }

    if (limit == NULL){
        WARNING("not CGI");
        return (false);
        //throw std::invalid_argument("not CGI");
    }

    const ConfigCgi *config_cgi = limit->cgi();
    if (config_cgi == NULL){
        WARNING("not CGI");
        return (false);
        throw std::invalid_argument("not CGI");
    }

    const std::map<std::string, std::string> &ext = config_cgi->get_extensions();
    std::map<std::string, std::string>::const_iterator ite = ext.begin();
    std::map<std::string, std::string>::const_iterator end = ext.end();

    std::string const filepath = req->requested_filepath();
    size_t pos = filepath.rfind("/");
    std::string filename = filepath.substr(pos+1);

    while(ite != end){
        const std::string &file_ext = ite->first;
        pos = filename.rfind(file_ext);
        if (pos != std::string::npos && (pos + file_ext.size() == filename.size())){
            MYINFO("CGI::is_cgi execve:" + ite->second);
            MYINFO("CGI::is_cgi filepath:" + filepath);
            return (true);
            //return ite->second;
            //return ;
        }
        ite++;
    }
    WARNING("not CGI");
    //throw std::invalid_argument("not CGI");
    return (false);
}

Request *WebservMakeRequestEvent::make_request(WebservEntity *entity)
{
    Request *req = Request::from_fd(entity->fd());
    this->parse_request(req);

    const ConfigServer *server = this->cfg->get_server(req);
    const ConfigLocation *location = this->cfg->get_location(server, req);

    req->set_requested_filepath(location);
    this->check_body_size(req, server);
    req->set_cgi(check_cgi(req, location));

    return (req);
}



/*
FileDiscriptor &WebservMakeRequestEvent::fd()
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
*/

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
    this->entity_->set_request(static_cast<Request*>(file));
}

WebservEntity*WebservMakeRequestEvent::entity()
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

