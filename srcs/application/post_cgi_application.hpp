#ifndef POST_CGI_APPLICATION_HPP
#define POST_CGI_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "webserv_file.hpp"

class PostCGIApplication : public Application
{
    public:
        static PostCGIApplication *get_instance();
        ~PostCGIApplication();
        WebservEvent* next_event(WebservEvent *event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEntity *event);
        bool execute(WebservEntity *entity);
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;

    private:
        PostCGIApplication();
        static PostCGIApplication *singleton;
        CGI *cgi;

        Method const method;
};
#endif
