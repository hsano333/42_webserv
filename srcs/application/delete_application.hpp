#ifndef DELETE_APPLICATION_HPP
#define DELETE_APPLICATION_HPP
#include "webserv_event.hpp"
#include "application.hpp"
#include "application_result.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "request.hpp"
#include "webserv_file.hpp"

class DeleteApplication : public Application
{
    public:
        static DeleteApplication *get_instance();
        ~DeleteApplication();
        WebservEvent* next_event(WebservEvent *event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEntity *event);
        bool execute(WebservEntity *event);
        bool is_cgi() const;

        const Method &which() const;
    private:
        DeleteApplication();
        static DeleteApplication *singleton;
        Method const method;
};
#endif
