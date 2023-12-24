#include "webserv_parser.hpp"
#include "http_exception.hpp"
#include "webserv_application_event.hpp"

WebservParser::WebservParser(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager,
            WebservEventFactory *event_factory,
            Config *cfg
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager),
        event_factory(event_factory),
        cfg(cfg)
{
    ;
}

WebservParser::~WebservParser()
{
;
}

bool WebservParser::check_body_size(Request *req, const ConfigServer *server)
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

void WebservParser::parse_req(WebservEvent *event)
{
    DEBUG("WebservParser::parse_req()");
    Request *req = event->req();
    /*
    char *body_p = Utility::strnstr(req->buf(), "\r\n\r\n", (size_t)req->buf_size());

    if(body_p == NULL && req->buf_size() >= MAX_READ_SIZE){
        body_p = Utility::strnstr(req->buf(), "\r\n",(size_t)req->buf_size());
        if(body_p){
            ERROR("Invalid Request. Heade is too long ");
            throw HttpException("431");
        }
        ERROR("Invalid Request.  request line is too long ");
        throw HttpException("414");
    }
    */
    //cout << "req->buf_size():" << req->buf_size() << endl;
    //char *buf_p = req->buf();
    //buf_p[req->buf_size()] = '\0';
    //printf("buf_p=%s\n", buf_p);
    /*
    if(body_p == NULL){
        DEBUG("WebservParser:: not still read Request header");
        event->set_completed(false);
        return ;
    }

    body_p += 4;

    MYINFO("WebservParser::parse_req:: req->buf_size()=" + Utility::to_string(req->buf_size()));
    MYINFO("WebservParser::parse_req:: (size_t)(body_p - req->buf())=" + Utility::to_string((size_t)(body_p - req->buf())));
    MYINFO("WebservParser::parse_req:: body_p=" + Utility::to_string(body_p));
    MYINFO("WebservParser::parse_req:: req->buf()=" + Utility::to_string(req->buf()));

    req->set_buf_body(body_p, req->buf_size() - (size_t)(body_p - req->buf()));
    body_p -= 4;
    *body_p = '\0';
    */

    Split sp(req->buf(), "\r\n");
    if(sp.size() == 0){
        delete (event);
        ERROR("Invalid Request. Reques doesn't have \"\r\n\"");
        //std::runtime_error("Invalid Request");
        throw HttpException("400");
    }
    try{
        req->set_request_line(sp[0]);
        req->set_header(sp, 1);
    }catch (std::invalid_argument &e){
        ERROR("Invalid Request:" + string(e.what()));
        throw HttpException("400");
    }

    req->print_info();
    event->set_completed(true);


    //Request *req = event->req();
    const ConfigServer *server = cfg->get_server(req);
    const ConfigLocation *location= this->cfg->get_location(server, req);
    req->set_requested_filepath(location);

    this->check_body_size(req, server);
}

void WebservParser::parse_res(WebservEvent *event)
{
    DEBUG("WebservParser::parse_res()");

    io_multi_controller->modify(event->fd(), EPOLLOUT | EPOLLONESHOT);
    Response *res = event->res();
    //printf("res=%p\n", res);
    WebservEvent *next_event = event_factory->make_write_event(event, res);
    this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);
    //this->event_manager->erase_event_waiting_epoll(event->fd());
    delete (event);
}
