#ifndef WEBSERV_APPLICATION_EVENT_HPP
#define WEBSERV_APPLICATION_EVENT_HPP
#include "application.hpp"
#include "application_factory.hpp"

#include "webserv_event.hpp"
#include "request.hpp"
#include "file_discriptor.hpp"
#include "application_result.hpp"
#include "webserv_application_event.hpp"
#include "webserv_entity.hpp"
#include "webserv_cleaner.hpp"

class WebservApplicationEvent
{
    public:
        ~WebservApplicationEvent();
        static WebservEvent *from_event(WebservEvent *event);
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *event_factory);
        void         set_next_epoll_event(E_EpollEvent epoll_event);
        E_EpollEvent epoll_event(WebservEvent *event);
        void        check_completed(WebservEntity * entity);
        static void delete_myself();

    private:
        WebservApplicationEvent();
        static WebservApplicationEvent *singleton;
        static WebservApplicationEvent *get_instance();
};

namespace App{
    template<typename EventT>
    bool invoke(EventT *event, WebservEntity *entity)
    {
        DEBUG("WEBSERV_APPLICATION_EVENT:: invoke");
        (void)event;
        ApplicationFactory *factory = ApplicationFactory::get_instance();
        bool is_completed;

        Application *app = factory->make_application(entity);
        entity->set_app(app);
        is_completed = app->execute(entity);

        entity->set_completed(is_completed);
        return (is_completed);
    }
};

#endif
