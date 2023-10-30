#include "webserv_parser.hpp"
#include "http_exception.hpp"
#include "webserv_application_event.hpp"

WebservParser::WebservParser(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager,
            WebservEventFactory *event_factory
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager),
        event_factory(event_factory)
{
    ;
}

WebservParser::~WebservParser()
{
;
}

void WebservParser::parse_req(WebservEvent *event)
{
    DEBUG("WebservParser::parse_req()");
    Request *req = event->req();
    char *body_p = Utility::strnstr(req->buf(), "\r\n\r\n",(size_t)req->buf_size());
    //cout << "req->buf_size():" << req->buf_size() << endl;
    //char *buf_p = req->buf();
    //buf_p[req->buf_size()] = '\0';
    //printf("buf_p=%s\n", buf_p);
    if(body_p == NULL){
        DEBUG("WebservParser:: not still read Request header");
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
    Split sp(req->buf(), "\r\n");
    if(sp.size() == 0){
        delete (event);
        ERROR("Invalid Request. Reques doesn't have \"\r\n\"");
        //std::runtime_error("Invalid Request");
        throw HttpException("400");
    }
    req->set_request_line(sp[0]);
    req->set_header(sp, 1);

    req->print_info();
    event->set_end(true);

    //delete (event);
    //req->print_info();
}

void WebservParser::parse_res(WebservEvent *event)
{
    DEBUG("WebservParser::parse_res()");

    io_multi_controller->modify(event->fd(), EPOLLOUT);
    Response *res = event->res();
    //printf("res=%p\n", res);
    WebservEvent *next_event = event_factory->make_write_event(event, res);
    this->event_manager->add_event_waiting_epoll(next_event->fd(), next_event);
    //this->event_manager->erase_event_waiting_epoll(event->fd());
    delete (event);
}
