

#include "webserv_executer.hpp"
#include "request.hpp"
#include "http_exception.hpp"

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

Application *WebservExecuter::get_application(WebservApplicationEvent *event)
{
    //Request *req = event->req();
    return (factory->make_application(event, reader));
}

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

void WebservExecuter::execute(WebservEvent *event)
{
    DEBUG("WebservExecuter::execute");

    WebservApplicationEvent *app_event = dynamic_cast<WebservApplicationEvent*>(event);
    Application *app = this->get_application(app_event);
    bool is_completed = app->execute(event);
    event->set_completed(is_completed);
    File *result = app->get_result();

    ApplicationResult *result2 = static_cast<ApplicationResult*>(result);
    cout << "make No.3 status code=" << result2->status_code().to_string() << endl;

    event->set_dst(result);
    //event->set_completed(result->is_completed());


    //WebservApplicationEvent *app_event = static_cast<WebservApplicationEvent*>(event);
    //bool is_completed = this->get_application(event);
    //File *result = app->get_result();


    //File *result_file = dynamic_cast<File *>(result);


    //todo
    /*
    try{
        bool is_completed = app->execute();
        if(app->is_cgi()){
            app_event->set_cgi_event(app->cgi_event());
        }
        app_event->set_completed(is_completed);
        if(is_completed)
        {
            //Response *res = NULL;
            //res = app->make_response();
            //app_event->set_response(res);
            //MYINFO("this->is_completed_:" + Utility::to_string(app_event->is_completed()));
            //MYINFO("this->cgi is_cgi:" + Utility::to_string(app_event->cgi_event()->is_cgi()));
            //delete app;
        }
    }catch (HttpException &e){
        ERROR("WebservExecuter::execute(): failure to making response");
        delete app;
        throw HttpException(e.what());
    }
    delete app;
    */
}
