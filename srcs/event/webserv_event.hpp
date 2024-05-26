#ifndef WEBSERV_EVENT_HPP
#define WEBSERV_EVENT_HPP
#include "webserv_entity.hpp"
#include "epoll.hpp"
#include "file_discriptor.hpp"
#include "request.hpp"
#include "response.hpp"
#include "webserv_event_factory.hpp"
#include <memory>
#include <typeinfo>

typedef enum E_WebservEvent
{
    CLEAN_EVENT,
    KEEP_ALIVE_EVENT,
    OTHER_EVENT,
} EWebservEvent;

class EventConcept
{
    public:
        virtual ~EventConcept(){};
        virtual bool handle() const = 0;
        virtual E_EpollEvent epoll_event(WebservEvent* event) const = 0;
        virtual WebservEvent* make_next_event(WebservEvent *event, WebservEventFactory *factory) const = 0;
        virtual EWebservEvent which() const = 0;
        virtual void check_completed() = 0;
};

class WebservEntity;
class WebservKeepAliveEvent;
template<typename EventPointer, typename HandleStrategyPointer>
class OwningEventModel : public EventConcept
{
    public:
        OwningEventModel(EventPointer event, HandleStrategyPointer handler, WebservEntity *entity) : event_(event), handler_(handler), entity_(entity), event_type_(OTHER_EVENT){};
        OwningEventModel(EventPointer event, HandleStrategyPointer handler, WebservEntity *entity, EWebservEvent event_type) : event_(event), handler_(handler), entity_(entity), event_type_(event_type){};
        OwningEventModel(OwningEventModel const &model) : event_(model.event), handler_(model.handler), entity_(model.entity){};
        bool handle() const {return handler_(event_, entity_);}
        E_EpollEvent epoll_event(WebservEvent* event) const {return (event_->epoll_event(event));}
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *factory) const {return (event_->make_next_event(event, factory));}
        EWebservEvent which() const {return (event_type_);}
        void check_completed() {return event_->check_completed(this->entity_);}

    private:
        EventPointer event_;
        HandleStrategyPointer handler_;
        WebservEntity *entity_;
        EWebservEvent event_type_;
};

class WebservEvent
{
    public:
        template<typename EventPointer, typename HandleStrategyPointer>
        WebservEvent( EventPointer event, HandleStrategyPointer handler, WebservEntity *entity) : entity_(entity), updated_time_(std::time(NULL))
        {
            typedef OwningEventModel<EventPointer, HandleStrategyPointer> Model;
            pimpl_ = new Model(event, handler, entity);
        }
        template<typename EventPointer, typename HandleStrategyPointer>
        WebservEvent( EventPointer event, HandleStrategyPointer handler, WebservEntity *entity, EWebservEvent event_type) : entity_(entity), updated_time_(std::time(NULL))
        {
            typedef OwningEventModel<EventPointer, HandleStrategyPointer> Model;
            pimpl_ = new Model(event, handler, entity, event_type);
        }

        ~WebservEvent(){delete pimpl_;};
        WebservEntity *entity(){return(this->entity_);}

        template<typename EventPointer>
        EventPointer event(){return (this->pimpl_);};
        void update_time();
        std::time_t last_updated_time();
        bool check_timeout(std::time_t now);
        bool check_died_child();


        E_EpollEvent epoll_event()
        {
            return (pimpl_->epoll_event(this));
        }
        WebservEvent* make_next_event(WebservEvent* event, WebservEventFactory *factory)
        {
            return (pimpl_->make_next_event(event, factory));
        }
        EWebservEvent which()
        {
            return (pimpl_->which());
        }
        void check_complted(){(pimpl_->check_completed());};

    private:
        WebservEntity *entity_;
        std::time_t updated_time_;

        friend void handle(WebservEvent *event)
        {
            event->pimpl_->handle();
            event->updated_time_ = std::time(NULL);
        }

        friend WebservEvent* make_next_event(WebservEvent *event, WebservEventFactory *factory)
        {
            return (event->pimpl_->make_next_event(event, factory));
        }

        EventConcept *pimpl_;
};

template<class EventT>
bool dummy_func(EventT *event, WebservEntity *entity)
{
    (void)event;
    (void)entity;
    entity->set_completed(true);
    return (true);
}

#endif
