#ifndef DELETE_APPLICATION_HPP
#define DELETE_APPLICATION_HPP
#include "application.hpp"
#include "application_result.hpp"
#include "cgi.hpp"
#include "config_location.hpp"
#include "config.hpp"
#include "request.hpp"
#include "file.hpp"

class DeleteApplication : public Application
{
    public:
        DeleteApplication();
        ~DeleteApplication();
        void init(WebservEvent *event);
        bool execute(WebservEvent *event);
        bool invoke(WebservEntity *entity);
        ApplicationResult *get_result();
        static DeleteApplication* from_location(const Config *cfg, const Request *req);
        Response* make_response();
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;

        const Method &which() const;
    private:
        File *get_requested_file();
        const Config *cfg;
        const ConfigServer *server;
        const ConfigLocation *location;
        const Request *req;
        std::map<std::string, std::string> tmp_headers;
        Method method;
};
#endif
