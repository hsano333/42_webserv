#ifndef WEBSERV_EVENT_ENTITY_HPP
#define WEBSERV_EVENT_ENTITY_HPP
//#include "epoll.hpp"
//#include "file_discriptor.hpp"
//#include "request.hpp"
//#include "response.hpp"
//#include "webserv_event_factory.hpp"
//#include "webserv_cgi_event.hpp"


/*
struct Entity
{
    Request             *req;
    Response            *res;
    ApplicationResult   *app;
};
*/

class WebservEventEntity
{
    public:
        struct Entity;
        WebservEventEntity(Entity *pimpl);
        ~WebservEventEntity();
        Entity *entity;
    private:
};

#endif
