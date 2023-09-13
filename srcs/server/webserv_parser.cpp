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
    (void)event;
    //Request *req = event->req();
    //io_multi_controller->modify(event->get_fd(), EPOLLOUT);
    WebservEvent *next_event = event_factory->make_application_event(event);
    delete (event);
    event_manager->push(next_event);
}

void WebservParser::parse_res(WebservEvent *event)
{
    DEBUG("WebservParser::parse_res()");
    Response *res = new Response();
    WebservEvent *next_event = event_factory->make_write_event(event, res);
    delete (event);
    this->event_manager->add_event_waiting_writing(next_event->get_fd(), next_event);
}