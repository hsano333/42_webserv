#ifndef GET_APPLICATION_HPP
#define GET_APPLICATION_HPP
#include "webserv_event.hpp"
#include "application.hpp"
#include "application_result.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "request.hpp"
#include "webserv_file.hpp"
#include "status_code.hpp"
#include "file_discriptor.hpp"

class GetApplication : public Application
{
    public:
        static GetApplication *get_instance();
        ~GetApplication();
        WebservEvent* next_event(WebservEvent *event, WebservEventFactory *event_factory);
        E_EpollEvent epoll_event(WebservEntity *event);
        bool execute(WebservEntity *event);
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;
        StatusCode const &code() const;
        std::map<string, string> const &header() const;

    private:
        GetApplication();
        static GetApplication *singleton;

        string get_index_path(Request *req, ConfigLocation const *location, bool *is_existed);
        string get_default_index_path(Request *req, bool *is_existed);
        Method const method;
        string check_content(string const &filepath);

};
#endif
