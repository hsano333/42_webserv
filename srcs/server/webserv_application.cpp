
#include "webserv_application.hpp"

WebservApplication::WebservApplication(
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager
        ):
        io_multi_controller(io_multi_controller),
        event_manager(event_manager)
{
    ;
}

WebservApplication::~WebservApplication()
{
    ;
}

void WebservApplication::process(WebservEvent *event)
{
    (void)event;
    ;
}
