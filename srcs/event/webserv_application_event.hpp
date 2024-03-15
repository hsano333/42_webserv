#ifndef WEBSERV_APPLICATION_EVENT_HPP
#define WEBSERV_APPLICATION_EVENT_HPP
#include "application.hpp"
#include "application_factory.hpp"
#include "webserv_entity.hpp"

template<typename EventT>
bool invoke(EventT *event, WebservEntity *entity)
{
    DEBUG("WEBSERV_APPLICATION_EVENT:: invoke");
    (void)event;
    ApplicationFactory *factory = ApplicationFactory::get_instance();
    bool is_completed;

    //if(entity->app() == NULL){
        //Application *app = factory->make_application(entity);
        //is_completed = app->init(entity);
    //}else{
    Application *app = factory->make_application(entity);
    entity->set_app(app);
    is_completed = app->execute(entity);
    //}

    entity->set_completed(is_completed);
    return (is_completed);
}

#endif
