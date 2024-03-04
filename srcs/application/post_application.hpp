#ifndef POST_APPLICATION_HPP
#define POST_APPLICATION_HPP
#include "application.hpp"
//#include "cgi.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "webserv_file.hpp"
#include "webserv_application_with_cgi_event.hpp"
#include "webserv_application_without_cgi_event.hpp"

class PostApplication : public Application
{
    public:
        PostApplication();
        ~PostApplication();
        bool execute(WebservEvent *event);
        bool invoke(WebservEntity *entity);
        ApplicationResult *get_result();
        static PostApplication* from_location(const Config *cfg, WebservEvent *event);
        Response* make_response(FileDiscriptor const &fd);
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;

    private:
        WebservFile *get_requested_file(FileDiscriptor const &fd);
        WebservEvent *event;
        bool check_not_cgi_end(size_t sum);
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        Response *res;
        std::map<std::string, std::string> tmp_headers;
        Method method;
};
#endif
