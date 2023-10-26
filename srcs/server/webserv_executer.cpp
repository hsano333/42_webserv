/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_executer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 14:33:57 by hsano             #+#    #+#             */
/*   Updated: 2023/10/27 02:12:09 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "webserv_executer.hpp"
#include "request.hpp"
#include "http_exception.hpp"

WebservExecuter::WebservExecuter(
            ApplicationFactory *factory,
            IOMultiplexing *io_multi_controller,
            WebservEventFactory *event_factory,
            EventManager *event_manager,
            FDManager *fd_manager,
            Config *cfg
        ):
        factory(factory),
        io_multi_controller(io_multi_controller),
        event_factory(event_factory),
        event_manager(event_manager),
        fd_manager(fd_manager),
        cfg(cfg)
{
    ;
}

WebservExecuter::~WebservExecuter()
{
    ;
}

std::string identify_path(URI &uri)
{
    (void)uri;
    return ("test");
}

Application *WebservExecuter::get_application(WebservApplicationEvent *event)
{
    Request *req = event->req();
    return (factory->make_application(req));
}

void WebservExecuter::execute(WebservEvent *event)
{
    DEBUG("WebservExecuter::execute");
    WebservApplicationEvent *app_event = static_cast<WebservApplicationEvent*>(event);

    Application *app = this->get_application(app_event);
    app->execute();
    Response *res;
    try{
        res = app->make_response();
    }catch (HttpException &e){
        delete app;
        throw HttpException(e.what());

    }
    //printf("res=%p\n", res);
    app_event->set_response(res);
    delete app;

    //io_multi_controller->modify(event->fd(), EPOLLOUT);
    //event_manager->erase_event_waiting_reading(event->fd());
    //WebservEvent *next_event = event_factory->make_write_event(event, res);
    //this->event_manager->add_event_waiting_writing(next_event->fd(), next_event);
    //event_manager->push(next_event);
    //delete (event);
}
