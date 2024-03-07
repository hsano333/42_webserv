#ifndef WEBSERV_APPLICATION_EVENT_HPP
#define WEBSERV_APPLICATION_EVENT_HPP
#include "application.hpp"
#include "application_factory.hpp"
#include "webserv_entity.hpp"

template<typename EventT>
bool invoke(EventT *event, WebservEntity *entity)
{
    (void)event;
    DEBUG("WebservExecuter::execute");
    ApplicationFactory *factory = ApplicationFactory::get_instance();
    DEBUG("WebservExecuter::execute No.1");
    Application *app = factory->make_application(entity);
    DEBUG("WebservExecuter::execute No.2");

    bool is_completed = app->invoke(entity);
    DEBUG("WebservExecuter::execute No.3");
    //ApplicationResult *result = app->get_result();

    //entity->set_result(result);
    entity->set_completed(is_completed);
    DEBUG("WebservExecuter::execute No.4");
    delete app;
    DEBUG("WebservExecuter::execute No.5");
    return (is_completed);
}

#endif
