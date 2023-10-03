#include "webserv_parser.hpp"
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
    cout << "req->buf_size():" << req->buf_size() << endl;
    char *buf_p = req->buf();
    buf_p[req->buf_size()] = '\0';
    printf("buf_p=%s\n", buf_p);
    if(body_p == NULL){
        DEBUG("WebservParser:: not still read Request header");
        //not delete( because still use)
        //delete event;
        return ;
    }
    body_p += 2;
    req->set_buf_body(body_p, req->buf_size()-(size_t)(body_p - req->buf()));
    body_p -= 2;
    *body_p = '\0';
    Split sp(req->buf(), "\r\n");
    if(sp.size() == 0){
        ERROR("Invalid Request. Reques doesn't have \"\r\n\"");
        std::runtime_error("Invalid Request");
    }
    req->set_request_line(sp[0]);
    WebservEvent *next_event = event_factory->make_application_event(event);
    delete (event);
    event_manager->push(next_event);
}

void WebservParser::parse_res(WebservEvent *event)
{
    DEBUG("WebservParser::parse_res()");


    io_multi_controller->modify(event->get_fd(), EPOLLOUT);
    Response *res = new Response();
    WebservEvent *next_event = event_factory->make_write_event(event, res);
    this->event_manager->add_event_waiting_writing(next_event->get_fd(), next_event);
    this->event_manager->erase_event_waiting_reading(event->get_fd());
    delete (event);
}
