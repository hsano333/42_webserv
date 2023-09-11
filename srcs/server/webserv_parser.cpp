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

void WebservParser::parse(WebservEvent *event)
{
    DEBUG("WebservParser::parse()");
    (void)event;
    Request *req = event->req;
    //io_multi_controller->modify(event->get_fd(), EPOLLOUT);
    WebservEvent *next_event = event_factory->make_application_event(event->get_fd(), req);
    delete (event);
    event_manager->push(next_event);
}
