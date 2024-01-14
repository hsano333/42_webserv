#ifndef APPLICATION_HPP
#define APPLICATION_HPP
#include "response.hpp"
#include "webserv_cgi_event.hpp"
#include "method.hpp"
#include "webserv_event.hpp"
//j#include "webserv_application_with_cgi_event.hpp"
//j#include "webserv_application_without_cgi_event.hpp"
#include "application_result.hpp"
//#include "webserv_application_event.hpp"

class ApplicationResult;
class Application
{
    public:
        //Application();
        virtual ~Application(){};
        //virtual bool execute(WebservApplicationEvent *event) = 0;
        //virtual bool execute() = 0;
        //virtual bool execute(WebservApplicationEvent *event) = 0;
        virtual bool execute(WebservEvent *event) = 0;
        //virtual bool execute(WebservApplicationEvent *event) = 0;
        virtual ApplicationResult *get_result() = 0;
        virtual bool is_cgi() const = 0;
        virtual Response* make_response() = 0;
        virtual WebservCgiEvent *cgi_event() = 0;
        //virtual void set_result(ApplicationResult *result) = 0;
        virtual const Method &which() const = 0;

        //virtual StatusCode &code() = 0;
        //virtual std::map<string, string> &header() = 0;

    private:

};
#endif
