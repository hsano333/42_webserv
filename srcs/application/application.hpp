#ifndef APPLICATION_HPP
#define APPLICATION_HPP

//#include "webserv_event.hpp"
//#include "webserv_event.hpp"
#include "response.hpp"
#include "method.hpp"
#include "application_result.hpp"
#include "webserv_entity.hpp"
#include "epoll.hpp"

class WebservEntity;
class Application
{
    public:
        virtual ~Application(){};
        //virtual WebservEvent* next_event(WebservEntity *entity, WebservEventFactory *event_factory) = 0;
        //virtual E_EpollEvent next_epoll_event(WebservEvent *event) = 0;
        virtual bool execute(WebservEntity *entity) = 0;
        virtual bool is_cgi() const = 0;
        virtual const Method &which() const = 0;
    private:

};
#endif
