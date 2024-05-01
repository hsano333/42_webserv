#ifndef POST_CGI_APPLICATION_HPP
#define POST_CGI_APPLICATION_HPP
#include "application.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "webserv_file.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"

class PostCGIApplication : public Application
{
    public:
        static PostCGIApplication *get_instance();
        //PostCGIApplication();
        ~PostCGIApplication();
        WebservEvent* next_event(WebservEvent *event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEntity *event);
        bool execute(WebservEntity *entity);
        ApplicationResult *get_result();
        void check_permission();
        static PostCGIApplication* from_location(const Config *cfg, WebservEvent *event, CGI *cgi);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;
    private:
        PostCGIApplication();
        static PostCGIApplication *singleton;
        WebservFile *get_requested_file(FileDiscriptor const &fd);
        WebservEvent *event;
        bool execute_cgi();
        bool execute_not_cgi();
        bool check_not_cgi_end(size_t sum);
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        Response *res;
        CGI *cgi;
        std::map<std::string, std::string> tmp_headers;
        std::string path_info_;
        Method const method;
};
#endif
