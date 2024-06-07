#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "webserv_event.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_entity.hpp"
#include "response.hpp"
#include "method.hpp"
#include "application_result.hpp"
#include "epoll.hpp"

class WebservEvent;
class WebservEntity;
class Application
{
    public:
        virtual ~Application(){};
        virtual WebservEvent* next_event(WebservEvent *event, WebservEventFactory *event_factory) = 0;
        virtual E_EpollEvent epoll_event(WebservEntity *entity) = 0;
        virtual bool execute(WebservEntity *entity) = 0;
        virtual bool is_cgi() const = 0;
        virtual const Method &which() const = 0;
    private:

};
#endif
