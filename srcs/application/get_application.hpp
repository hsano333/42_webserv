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
        //bool execute(WebservEvent *event);
        bool invoke(WebservEntity *entity);
        //ApplicationResult *get_result();
        //static GetApplication* from_location(const Config *cfg, const Request *req);
        //Response* make_response(FileDiscriptor const &fd);
        void set_path_info(std::string const &path_info);
        bool is_cgi() const;
        const Method &which() const;

        StatusCode const &code() const;
        std::map<string, string> const &header() const;

    private:
        GetApplication();
        static GetApplication *singleton;
        //static WebservCleanEvent *get_instance();

        string get_index_path(Request *req, ConfigLocation const *location, bool *is_existed);
        //WebservFile *get_requested_file(FileDiscriptor const &fd);

        //const Config *cfg;
        //const ConfigServer *server;
        //const ConfigLocation *location;
        //const Request *req;
        //std::map<std::string, std::string> tmp_headers;
        //std::string path_info_;
        Method method;
        string check_content(string const &filepath);

        //std::map<std::string, std::string> header_;
        //ApplicationResult *result_;

};
#endif
