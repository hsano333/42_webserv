#ifndef GET_CGI_APPLICATION_HPP
#define GET_CGI_APPLICATION_HPP
#include "webserv_event.hpp"
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "request.hpp"
#include "webserv_file.hpp"

class GetCGIApplication : public Application
{
    public:
        static GetCGIApplication *get_instance();
        ~GetCGIApplication();
        WebservEvent* next_event(WebservEvent *event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEntity *event);
        bool execute(WebservEntity *entity);
        //ApplicationResult *get_result();
        //static GetCGIApplication* from_location();
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;

    private:
        GetCGIApplication();
        static GetCGIApplication *singleton;
        //const Config *cfg;
        //const ConfigServer *server;
        //const ConfigLocation *location;
        //const Request *req;
        CGI *cgi;
        //std::map<std::string, std::string> tmp_headers;
        //std::string path_info_;

        const Method method;
        //ApplicationResult *result_;
};
#endif
