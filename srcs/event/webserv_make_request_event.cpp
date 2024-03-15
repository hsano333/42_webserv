#include "webserv_make_request_event.hpp"
#include "webserv_event.hpp"
#include "http_exception.hpp"
#include "socket_file.hpp"

WebservMakeRequestEvent::WebservMakeRequestEvent()
{
    ;
}

WebservMakeRequestEvent::~WebservMakeRequestEvent()
{
};

namespace free_func{
    bool make_request(WebservMakeRequestEvent *event, WebservEntity *entity)
    {
        Request *req = event->make_request(entity);
        entity->set_request(req);
        entity->set_completed(true);
        return (true);
    }
}

WebservMakeRequestEvent *WebservMakeRequestEvent::singleton = NULL;
WebservMakeRequestEvent *WebservMakeRequestEvent::get_instance()
{
    if (WebservMakeRequestEvent::singleton == NULL){
        singleton = new WebservMakeRequestEvent();
    }
    return (singleton);
}


WebservEvent *WebservMakeRequestEvent::from_event(WebservEvent *event, WebservFile *src, WebservFile *dst)
{
    DEBUG("WebservMakeRequestEvent::from_event");
    WebservMakeRequestEvent *req_event = WebservMakeRequestEvent::get_instance();
    WebservEvent *new_event =  new WebservEvent( req_event, free_func::make_request, event->entity());
    new_event->entity()->io().set_source(src);
    new_event->entity()->io().set_destination(dst);

    return (new_event);
};

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


void WebservMakeRequestEvent::parse_request(Request *req, WebservFile *src)
{
    DEBUG("WebservMakeRequestEvent::parse_request");
    char *buf_p;

    size_t buf_size = src->read(&buf_p, MAX_REAUEST_EXCEPT_BODY);
    (void)buf_size;
    cout << "buf_size=" << buf_size << endl;
    buf_p[buf_size] = '\0';
    cout << "buf_p=[" << buf_p << "]" << endl;
    if(buf_size == 0){
        ERROR("Invalid Request. ");
        throw HttpException("400");
    }

    Split sp0(buf_p, CRLF2);
    cout << "sp0.size=" << sp0.size() << endl;
    int index = 0;
    if(sp0.size() > 1){
        index = 1;
        req->set_buf_body(sp0[index].c_str(), sp0[index].size());
        //ERROR("Invalid Request. Reques doesn't have \"\r\n\r\n\"");
        //throw HttpException("400");
    }
    try{
        Split sp1(sp0[0], CRLF);
        req->set_request_line(sp1[0]);
        req->set_header(sp1, 1);
    }catch (std::invalid_argument &e){
        ERROR("Invalid Request:" + string(e.what()));
        throw HttpException("400");
    }
}

bool WebservMakeRequestEvent::check_cgi(const Request *req, const ConfigLocation *location) const
{
    DEBUG("WebservMakeRequestEvent::check_cgi");
    (void)req;
    (void)location;
    const ConfigLimit *limit = location->limit();

    if (req->is_file() == false){
        MYINFO("not CGI");
        return (false);
    }

    if (limit == NULL){
        MYINFO("not CGI");
        return (false);
    }

    const ConfigCgi *config_cgi = limit->cgi();
    if (config_cgi == NULL){
        MYINFO("not CGI");
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
            MYINFO("CGI!!!!!!!!!!!!");
            return (true);
        }
        ite++;
    }
    MYINFO("not CGI");
    return (false);
}

Request *WebservMakeRequestEvent::make_request(WebservEntity *entity)
{
    DEBUG("WebservMakeRequestEvent::make_request()");
    Request *req = Request::from_fd(entity->fd());
    this->parse_request(req, entity->io().source());

    const ConfigServer *server = entity->config()->get_server(req);
    const ConfigLocation *location = entity->config()->get_location(server, req);

    req->set_requested_filepath(location);
    this->check_body_size(req, server);
    req->set_cgi(check_cgi(req, location));

    return (req);
}

void WebservMakeRequestEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
