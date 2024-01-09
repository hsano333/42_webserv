#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "fd_manager.hpp"
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "port.hpp"
#include "config_http.hpp"
#include "config_server.hpp"
#include "config_location.hpp"
#include "request.hpp"
#include "file.hpp"

#include "socket_repository.hpp"
#include "file_discriptor.hpp"

//#include "config_repository.hpp"
//#include "config_factory.hpp"
//
class Request;

class Config : public ConfigObject<ConfigHttp*>
{
    public:
        //ConfigRepository& repository;
        //ConfigRepositoryFactory& factory;

        Config(FDManager *fd_manager);
        Config();
        ~Config();

        void load();
        static Config *get_instance();
        static void set_filepath(const char *filepath);
        ConfigHttp *http;
        void retrieve(const std::string &word, std::map<std::string, std::string> &map, void(*f)(char *str));
        bool getline(std::string &str);
        const std::string content_word;

        const ConfigServer *get_server(Request const *req) const;
        const ConfigServer *get_server(Port const& port, std::string const& host) const;
        const ConfigLocation *get_location(const ConfigServer *server, const Request *req) const;
        std::vector<std::string> get_location_paths(Port const& port, std::string const& host) const;
        //std::map<std::string, std::vector<std::string> > get_locations_contents(Port const& port, std::string const& host,
                                                            //std::string const& location) const;
        //std::map<std::string, std::vector<std::string> > get_locations_properties(Port const& port, std::string const& host,
                                                              //std::string const& filepath) const;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigHttp*> const &vec);
        void check();
        void check(SocketRepository *socket_repository);

        File *get_error_file(Request const *req, StatusCode &code) const;

        void print_cfg();
    private:

        FDManager *fd_manager_;
        static Config *_singleton;
        static std::string _filepath;
        std::ifstream _fin;
        static std::map<std::pair<Port, std::string>, ConfigServer const*> servers_cache;
        static std::map<std::pair<ConfigServer const *, std::string>, ConfigLocation const * > locations_cache;
        //static std::map<std::pair<Port, std::string>, std::vector<std::string> > locations_cache;
        static std::map<std::pair<std::pair<Port, std::string>, std::string>, std::map<std::string, std::vector<std::string> > > _locations_content_cache;
        static std::map<std::pair<std::pair<Port, std::string>, std::string>, std::map<std::string, std::vector<std::string> > > _locations_properties_cache;

};
#endif
