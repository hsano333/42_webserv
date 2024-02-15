#ifndef WEBSERV_EXECUTER_HPP
#define WEBSERV_EXECUTER_HPP
#include "webserv_event.hpp"
#include "event_manager.hpp"
//#include "webserv_application_event.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"
#include "epoll_controller.hpp"
#include "application_factory.hpp"
#include "webserv_event_factory.hpp"
#include "socket_reader.hpp"

class WebservExecuter
{
    public:
        WebservExecuter(
                      ApplicationFactory *factory,
                      IOMultiplexing *io_multi_controller,
                      WebservEventFactory *event_factory,
                      EventManager *event_manager,
                      FDManager *fd_manager,
                      Config *cfg,
                      SocketReader *reader
                );
        ~WebservExecuter();
        //bool check_redirect(WebservApplicationEvent *event);
        //void invoke(WebservEvent *event);
    private:
        ApplicationFactory *factory;
        IOMultiplexing *io_multi_controller;
        WebservEventFactory *event_factory;
        EventManager *event_manager;
        FDManager *fd_manager;
        Config *cfg;
        Application *get_application(WebservEvent *event);
        SocketReader *reader;
};

/*
template<typename EventPointer>
void execute(EventPointer event)
{
    DEBUG("WebservExecuter::execute");

    //WebservApplicationEvent *app_event = dynamic_cast<WebservApplicationEvent*>(event);
    //Application *app = this->get_application(app_event);
    //app->init(event);
    //bool is_completed = app->execute(event);
    //event->set_completed(is_completed);
    File *result = event->execute();

    //ApplicationResult *result2 = static_cast<ApplicationResult*>(result);
    //cout << "make No.3 status code=" << result2->status_code().to_string() << endl;

    event->set_dst(result);
}
*/

template<typename EventT>
void invoke(EventT *event, WebservEntity *entity)
{
    (void)event;
    DEBUG("WebservExecuter::execute");
    ApplicationFactory *factory = ApplicationFactory::get_instance();
    Application *app = factory->make_application(entity);

    bool is_completed = app->invoke(entity);
    entity->set_completed(is_completed);
    ApplicationResult *result = app->get_result();

    entity->set_result(result);
    delete app;
}

#endif
