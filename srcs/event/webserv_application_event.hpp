#ifndef WEBSERV_APPLICATION_EVENT_HPP
#define WEBSERV_APPLICATION_EVENT_HPP
#include "application.hpp"
#include "application_factory.hpp"
#include "webserv_entity.hpp"

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
