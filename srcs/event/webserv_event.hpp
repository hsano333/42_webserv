#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP
#include "webserv_entity.hpp"
#include "epoll.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_event_factory.hpp"
#include "webserv_keep_alive_event.hpp"
#include <memory>
#include <typeinfo>

//class WebservEvent;
//class WebservEntity;
class EventConcept
{
    public:
        virtual ~EventConcept(){};
        virtual void handle() const = 0;
        virtual E_EpollEvent get_next_epoll_event(WebservEvent* event) const = 0;
        virtual WebservEvent* make_next_event(WebservEvent *event, WebservEventFactory *factory) const = 0;
        virtual bool is_keepalive() const = 0;
        //virtual bool is_completed() const = 0;
};

class WebservEntity;
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
        //bool is_completed() const {return (this->entity_->completed());}
        //bool is_completed() const {return (true);}
        E_EpollEvent get_next_epoll_event(WebservEvent* event) const {return (event_->get_next_epoll_event(event));}
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *factory) const {return (event_->make_next_event(event, factory));}

    private:
        EventPointer event_;
        HandleStrategyPointer handler_;
        WebservEntity *entity_;
};

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
        //bool completed(){return(this->entity_->completed());}

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
        //void set_completed(bool flag);
        //bool completed();

    private:
        WebservEntity *entity_;
        int timeout_count_;
        //bool completed_;

        //template<typename EventPointer, typename HandleStrategyPointer>
        friend void handle(WebservEvent *event)
        {
            event->pimpl_->handle();
        }
        EventConcept *pimpl_;
};

template<class EventT>
bool dummy_func(EventT *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
    return (true);
}

#endif
