/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv_executer.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hsano <hsano@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/15 03:01:59 by hsano             #+#    #+#             */
/*   Updated: 2024/02/15 03:48:26 by sano             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "webserv_executer.hpp"
#include "request.hpp"
#include "http_exception.hpp"
#include "get_cgi_application.hpp"
#include "get_cgi_application.hpp"
#include "post_application.hpp"
#include "post_cgi_application.hpp"
#include "delete_application.hpp"

WebservExecuter::WebservExecuter(
            ApplicationFactory *factory,
            IOMultiplexing *io_multi_controller,
            WebservEventFactory *event_factory,
            EventManager *event_manager,
            FDManager *fd_manager,
            Config *cfg,
            SocketReader *reader
        ):
        factory(factory),
        io_multi_controller(io_multi_controller),
        event_factory(event_factory),
        event_manager(event_manager),
        fd_manager(fd_manager),
        cfg(cfg),
        reader(reader)
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

/*
Application *WebservExecuter::get_application(WebservEvent *event)
{
    //Request *req = event->req();

    ApplicationFactory *factory = ApplicationFactory::get_instance();
    Application app = (factory->make_application());
    app->execute(event);
}
*/

/*
bool WebservExecuter::check_redirect(WebservApplicationEvent *event)
{
    DEBUG("WebservExecuter::check_redirect");

    Request *req = event->req();
    const ConfigServer *server = cfg->get_server(req);
    const ConfigLocation *location= this->cfg->get_location(server, req);

    if(!location->is_redirect()){
        return (false);
    }

    const std::pair<StatusCode, std::string> &redirect = location->redirect();

    StatusCode code = redirect.first;
    Response *res = NULL;
    res = Response::from_error_status_code(code);
    //cout << "redirect:" << redirect.second << endl;
    res->add_header(LocationHeader, redirect.second);
    res->add_header(CORS_ORIGIN, CORS_ORIGIN_VALUE);
    event->set_response(res);
    event->set_completed(true);

    MYINFO("WebservExecuter::redirect is valid to:" + redirect.second);
    return (true);
}
*/



