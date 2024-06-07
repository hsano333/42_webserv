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
#include "webserv_file.hpp"

#include "socket_repository.hpp"
#include "file_discriptor.hpp"

class Request;

class Config : public ConfigObject<ConfigHttp*>
{
    public:

        Config(FDManager *fd_manager);
        Config();
        ~Config();

        void load();
        static void set_filepath(const char *filepath);
        ConfigHttp *http;
        void retrieve(const std::string &word, std::map<std::string, std::string> &map, void(*f)(char *str));
        bool getline(std::string &str);
        const std::string content_word;

        const ConfigServer *get_server(Request const *req) const;
        const ConfigServer *get_server(Port const& port, std::string const& host) const;
        const ConfigLocation *get_location(const ConfigServer *server, const Request *req) const;
        std::vector<std::string> get_location_paths(Port const& port, std::string const& host) const;

        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigHttp*> const &vec);
        void check();
        void check() const;
        void check(SocketRepository *socket_repository);

        void print_cfg();
    private:

        FDManager *fd_manager_;
        static Config *_singleton;
        static std::string _filepath;
        std::ifstream _fin;
        static std::map<std::pair<Port, std::string>, ConfigServer const*> servers_cache;
        static std::map<std::pair<ConfigServer const *, std::string>, ConfigLocation const * > locations_cache;
        static std::map<std::pair<std::pair<Port, std::string>, std::string>, std::map<std::string, std::vector<std::string> > > _locations_content_cache;
        static std::map<std::pair<std::pair<Port, std::string>, std::string>, std::map<std::string, std::vector<std::string> > > _locations_properties_cache;

};
#endif
