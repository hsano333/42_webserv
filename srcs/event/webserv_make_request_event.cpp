#include "webserv_make_request_event.hpp"
#include "webserv_event.hpp"
#include "http_exception.hpp"
#include "socket_file.hpp"
#include "global.hpp"
#include "base64.hpp"
#include "split.hpp"

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
        (void)req;
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

void WebservMakeRequestEvent::delete_myself()
{
    if(WebservMakeRequestEvent::singleton)
        delete WebservMakeRequestEvent::singleton;
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
    DEBUG("WebservMakeRequestEvent::make_next_event");
    return (event_factory->make_application_event(event));
}

E_EpollEvent WebservMakeRequestEvent::epoll_event(WebservEvent *event)
{
    (void)event;
    return (EPOLL_NONE);
}

bool WebservMakeRequestEvent::check_body_size(Request *req, const ConfigServer *server)
{
    DEBUG("WebservMakeRequestEvent::check_body_size");

    ssize_t max_body_size = (ssize_t)server->get_max_body_size();
    Header const &header = req->header();
    ssize_t content_len = 0;

    try{
        content_len = header.get_content_length();
    }catch(std::invalid_argument &e){
        ERROR("Invalid Request Content-Length:" + header.get_content_length_str());
        throw HttpException("400");
    }
    if(content_len > 0){
        req->set_has_body(true);
    }

    size_t cur_body_size = 0;
    req->get_buf_body(&cur_body_size);
    bool is_chunk = header.is_chunked();

    if(!is_chunk){
        if(content_len <= 0 && cur_body_size > 0){
            ERROR("Invalid Request. Content-Length is not set but body exist. body size:" + Utility::to_string(cur_body_size));
            throw HttpException("411");
        }
        if(content_len > max_body_size){
            ERROR("Invalid Request. Content-Length is more than " + Utility::to_string(max_body_size));
            throw HttpException("413");
        }
    }else{
        req->set_has_body(true);
    }

    return (true);
}

void WebservMakeRequestEvent::check_auth(Request *req, const ConfigLocation *location)
{
    DEBUG("WebservMakeRequestEvent::check_auth");
    (void)req;
    (void)location;
    if(location->auth_basic() == false){
        return;
    }

    std::string const &path = location->auth_basic_path();
    if(Utility::is_readable_file(path) == false){
        ERROR("not readable auth basic file:" + path);
        throw HttpException("500");
    }

    Header const &header = req->header();
    std::string const &auth = header.find(AUTHORIZATION);
    if(auth == header.not_find()){
        throw HttpException("401");
    }
    Split sp(auth, " ");
    if(sp.size() < 2 || sp[1].size() == 0){
        throw HttpException("401");
    }


    std::ifstream ifs(path.c_str());
    if (ifs.is_open() == false){
        ERROR("not readable auth basic file:" + path);
        throw HttpException("500");
    }

    std::string decoded_auth = Base64::decode(sp[1]);
    std::string buf;
    while (getline(ifs, buf)) {
        if(decoded_auth == buf){
            Split sp(decoded_auth, ":");
            if(sp.size() == 2){
                req->set_auth(sp[0]);
            }else{
                throw HttpException("401");
            }
            return ;
        }
    }
    throw HttpException("401");
}

void WebservMakeRequestEvent::parse_request(Request *req, WebservFile *src)
{
    DEBUG("WebservMakeRequestEvent::parse_request");
    char *buf_p;

    ssize_t buf_size = src->read(&buf_p, MAX_REAUEST_EXCEPT_BODY);

    if(buf_size <= 0){
        ERROR("Invalid Request. ");
        throw HttpException("400");
    }
    buf_p[buf_size] = '\0';

    char *body_start = Utility::strnstr(buf_p, CRLF2, buf_size);

    if(body_start){
        body_start[0] = '\0';
        // +4 is \r\n\r\n
        body_start += 4;
        req->set_buf_body(body_start, buf_size - (body_start - buf_p));
    }else{
        ERROR("not found CRLF2");
        throw HttpException("400");

    }

    if(buf_size == 0){
        ERROR("Invalid Request. size=0");
        throw HttpException("400");

    }
    try{
        Split sp1(buf_p, CRLF);
        if(sp1.size() == 0){
            ERROR("not found statue line");
            throw HttpException("400");
        }
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
    Request *req = Request::from_fd(entity->fd());
    DEBUG("WebservMakeRequestEvent::make_request() address:" + Utility::to_string(req));
    try{
        this->parse_request(req, entity->io().source());
        const ConfigServer *server = entity->config()->get_server(req);
        const ConfigLocation *location = entity->config()->get_location(server, req);

        req->set_requested_filepath(location);
        this->check_body_size(req, server);
        this->check_auth(req, location);
        req->set_cgi(check_cgi(req, location));

        entity->set_request(req);

        if(location->is_redirect()){
            DEBUG("Redirect!");
            throw HttpException(location->redirect().first.to_string());
        }

    }catch(HttpException &e){
        ERROR("WebservMakeRequestEvent::make_request:" + Utility::to_string(e.what()));
        //delete req;
        throw HttpException(e.what());
    }

    return (req);
}

void WebservMakeRequestEvent::check_completed(WebservEntity * entity)
{
    entity->set_completed(true);
}
