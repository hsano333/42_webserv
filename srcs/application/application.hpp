#ifndef APPLICATION_HPP
#define APPLICATION_HPP

//#include "webserv_event.hpp"
#include "response.hpp"
#include "method.hpp"
#include "application_result.hpp"
#include "webserv_entity.hpp"

//class WebservEvent;
//class WebservEntity;
//class ApplicationResult;
class Application
{
    public:
        virtual ~Application(){};
        virtual bool invoke(WebservEntity *entity) = 0;
        //virtual ApplicationResult *get_result() = 0;
        virtual bool is_cgi() const = 0;
        virtual const Method &which() const = 0;

    private:

};
#endif
