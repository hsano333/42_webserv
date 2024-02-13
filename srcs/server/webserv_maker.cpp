#include "webserv_maker.hpp"
#include "http_exception.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"
#include "webserv_make_event.hpp"

WebservMaker::WebservMaker(
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

WebservMaker::~WebservMaker()
{
    ;
}

/*
bool WebservMaker::check_body_size(Request *req, const ConfigServer *server)
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

void WebservMaker::parse_req(WebservEvent *event)
{
    DEBUG("WebservMaker::parse_req()");
    File *req = event->req();

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

/*
void WebservMaker::make(WebservEvent *event)
{
    DEBUG("WebservMaker::make()");

    DEBUG("WebservMaker::make() No.1");
    event->entity()->config()->check();
    DEBUG("WebservMaker::make() No.2");
    event->entity()->config()->check();
    WebservMakeEvent *make_event = dynamic_cast<WebservMakeEvent*>(event);
    //WebservMakeEvent *make_event = const_cast<void*>(static_cast<void const volatile*>(event));

    DEBUG("WebservMaker::make() No.3");
    event->entity()->config()->check();
    DEBUG("WebservMaker::make() No.4");
    event->entity()->config()->check();



    DEBUG("WebservMaker::make() No.5");
    if(!make_event){
        cout << "NULL" << endl;
    }
    printf("event=%p\n", make_event);
    make_event->set_file(NULL);

    DEBUG("WebservMaker::make() No.6");
    event->entity()->config()->check();
    DEBUG("WebservMaker::make() No.7");
    event->entity()->config()->check();
    DEBUG("WebservMaker::make() No.8");



    File *file = make_event->make();
    make_event->set_file(file);
    event->set_completed(true);
}
*/


#include "webserv_make_request_event.hpp"
#include "webserv_make_response_event.hpp"

void WebservMaker::make(WebservEvent *event)
{
    DEBUG("WebservMaker(WebservMakeRequestEvent)::make()");
    handle(event);
    event->entity()->set_completed(true);
}


/*
void make(WebservMakeResponseEvent *event, WebservEntity *entity)
{
    DEBUG("WebservMaker(WebservMakeResponseEvent)::make()");
    entity->set_completed(false);
    File *file = event->handle();
    event->set_file(file);
    entity->set_completed(true);
}
*/


