#ifndef POST_APPLICATION_HPP
#define POST_APPLICATION_HPP
#include "application.hpp"
//#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "webserv_file.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"
#include "webserv_file_factory.hpp"

class PostApplication : public Application
{
    public:
        static PostApplication *get_instance();
        static PostApplication *get_instance(WebservFileFactory *file_factory);
        ~PostApplication();
        //bool execute(WebservEvent *event);
        WebservEvent* next_event(WebservEvent *event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEntity *event);
        bool execute(WebservEntity *entity);
        //bool init(WebservEvent *event, WebservEntity *entity);
        //bool upload(WebservEntity *entity);
        //ApplicationResult *get_result();
        //static PostApplication* from_location(const Config *cfg, WebservEvent *event);
        //Response* make_response(FileDiscriptor const &fd);
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;

    private:
        PostApplication();
        static PostApplication *singleton;
        //WebservFile *get_requested_file(FileDiscriptor const &fd);
        //WebservEvent *event;
        bool check_not_cgi_end(size_t sum);
        //const Config *cfg;
        //const ConfigServer *server;
        //const ConfigLocation *location;
        //const Request *req;
        //Response *res;
        //std::map<std::string, std::string> tmp_headers;
        Method const method;
        WebservFileFactory *file_factory;
};
#endif
