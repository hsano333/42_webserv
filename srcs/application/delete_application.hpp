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
        //bool init(WebservEntity *event);
        WebservEvent* next_event(WebservEvent *event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEntity *event);
        bool execute(WebservEntity *event);
        //bool invoke(WebservEntity *entity);
        //ApplicationResult *get_result();
        //static DeleteApplication* from_location(const Config *cfg, const Request *req);
        //Response* make_response();
        //void set_path_info(std::string const &path_info);
        bool is_cgi() const;

        const Method &which() const;
    private:
        DeleteApplication();
        static DeleteApplication *singleton;
        //WebservFile *get_requested_file(FileDiscriptor const &fd);
        //const Config *cfg;
        //const ConfigServer *server;
        //const ConfigLocation *location;
        //const Request *req;
        //std::map<std::string, std::string> tmp_headers;
        Method method;
};
#endif
