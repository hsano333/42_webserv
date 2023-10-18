/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_executer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 14:33:57 by hsano             #+#    #+#             */
/*   Updated: 2023/10/18 14:34:19 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "webserv_executer.hpp"
#include "request.hpp"

WebservExecuter::WebservExecuter(
            ApplicationFactory *factory,
            IOMultiplexing *io_multi_controller,
            EventManager *event_manager,
            FDManager *fd_manager,
            Config *cfg
        ):
        factory(factory),
        io_multi_controller(io_multi_controller),
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
    Response *res = app->make_response();
    app_event->set_response(res);
    delete app;

}
