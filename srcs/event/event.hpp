#ifndef EVENT_HPP
#define EVENT_HPP

//#include "handle_strategy.hpp"
//#include "owing_event_model.hpp"

#include <iostream>

class Event;
class EventConcept
{
    public:
        virtual ~EventConcept(){};
        virtual void handle() const = 0;
        virtual void entity() const = 0;

        //not use;
        //virtual ShapceConcept *clone() const = 0;
};

template<typename EventPointer, typename HandleStrategyPointer>
class OwningEventModel : public EventConcept
{
    public:
        OwningEventModel(EventPointer event, HandleStrategyPointer handler) : event_(event), handler_(handler){};
        void handle() const{handler_(event_);}

    private:
        EventPointer event_;
        HandleStrategyPointer handler_;
};

class Event
{
    public:
       template<typename EventPointer, typename HandleStrategyPointer>
       Event( EventPointer event, HandleStrategyPointer handler, Entity *entity) : entity_(entity)
       {
          //using Model = OwningEventModel<EventPointer, HandleStrategyPointer>;
          typedef OwningEventModel<EventPointer, HandleStrategyPointer> Model;
          pimpl_ = new Model(event, handler);
       }
       WebservEntity *entity(return (entity_));
    private:
        friend void handle(Event const &event)
        {
            event.pimpl_->handle();
        }
        EventConcept *pimpl_;
        Entity *entity_;
        //Entity *pimpl_;
        //Entity *get_entity(FileDiscriptor &fd){};
};

#endif
