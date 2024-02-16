#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP
#include "epoll.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_entity.hpp"
#include <memory>

typedef enum E_WebservEvent
{
    INIT_EVENT = 0,
    READ_EVENT,
    IO_EVENT,
    APPLICATION_EVENT,
    WRITE_EVENT,
    CLEAN_EVENT, //5
    TIMEOUT_EVENT,
    KEEPA_ALIVE_EVENT,
    NOTHING_EVENT,
    MAKE_EVENT, //9
} EWebservEvent;

typedef enum E_EpollEvent
{
    EPOLL_READ,
    EPOLL_WRITE,
    EPOLL_ADD_READ,
    EPOLL_ADD_WRITE,
    EPOLL_FOR_CGI,
    EPOLL_CONTINUE,
    EPOLL_CLOSE,
    EPOLL_CGI_IN,
    EPOLL_CGI_OUT,
    EPOLL_CGI_STOP,
    EPOLL_CGI,
    EPOLL_NONE,
} E_EpollEvent;



class WebservEvent;
class WebservEntity;
class EventConcept
{
    public:
        virtual ~EventConcept(){};
        virtual void handle() const = 0;
        virtual E_EpollEvent get_next_epoll_event(WebservEvent* event) const = 0;
        virtual WebservEvent* make_next_event(WebservEvent *event, WebservEventFactory *factory) const = 0;
        virtual bool is_keepalive() const = 0;

        //virtual void change_event(WebservEvent* event, HandleStrategyPointer handler_) const = 0;
};

#include <typeinfo>
#include "webserv_keep_alive_event.hpp"
class WebservKeepAliveEvent;
template<typename EventPointer, typename HandleStrategyPointer>
class OwningEventModel : public EventConcept
{
    public:
        OwningEventModel(EventPointer event, HandleStrategyPointer handler, WebservEntity *entity) : event_(event), handler_(handler), entity_(entity){};
        OwningEventModel(OwningEventModel const &model) : event_(model.event), handler_(model.handler), entity_(model.entity){};
        void handle() const {handler_(event_, entity_);}
        void clean() const {handler_(event_, entity_);}
        bool is_keepalive() const {return (typeid(*event_) == typeid(WebservKeepAliveEvent));}
        E_EpollEvent get_next_epoll_event(WebservEvent* event) const {return (event_->get_next_epoll_event(event));}
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *factory) const {return (event_->make_next_event(event, factory));}

    private:
        EventPointer event_;
        HandleStrategyPointer handler_;
        WebservEntity *entity_;
};



//namespace EVENT
//{

class WebservEntity;
class WebservEvent
{
    public:
        template<typename EventPointer, typename HandleStrategyPointer>
        WebservEvent( EventPointer event, HandleStrategyPointer handler, WebservEntity *entity) : entity_(entity)
        {
            typedef OwningEventModel<EventPointer, HandleStrategyPointer> Model;
            pimpl_ = new Model(event, handler, entity);
        }

        ~WebservEvent(){delete pimpl_;};
        WebservEntity *entity(){return(this->entity_);}

        template<typename EventPointer>
            EventPointer event(){return (this->pimpl_);};
        int  timeout_count();
        void increase_timeout_count(int count);

        E_EpollEvent get_next_epoll_event()
        {
            return (pimpl_->get_next_epoll_event(this));
        }
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *factory)
        {
            return (pimpl_->make_next_event(event, factory));
        }
        bool is_keepalive()
        {
            return (pimpl_->is_keepalive());
        }
        //void clean()
        //{
            //delete entity_;
        //}

    private:
        WebservEntity *entity_;
        int timeout_count_;

        friend void handle(WebservEvent const *event)
        {
            event->pimpl_->handle();
        }
        /*
           friend void make(WebservEvent const *event)
           {
           event->pimpl_->handle();
           }
           friend void invoke(WebservEvent const *event)
           {
           event->pimpl_->handle();
           }
           */
        EventConcept *pimpl_;
};
//}

namespace EVENT_DUMMY
{
    template<typename EventPointer, typename HandleStrategyPointer>
        void func(EventPointer *event, WebservEntity *entity)
        {
            (void)event;
            (void)entity;
        }
}
//using EVENT;

#endif
