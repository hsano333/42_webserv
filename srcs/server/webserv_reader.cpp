#include "webserv_reader.hpp"

WebservReader::WebservReader(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager)

{
    ;
}

WebservReader::~WebservReader()
{
    ;
}

void WebservReader::read(WebservEvent *event)
{
    (void)event;
    io_multi_controller->modify(event->get_fd(), EPOLLOUT);
}
