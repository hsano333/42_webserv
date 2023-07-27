#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <fstream>
#include <vector>
#include <map>
#include "config_http.hpp"
#include "config_server.hpp"
#include "config_location.hpp"

class Config
{
    public:
        static Config *get_instance();
        ~Config();
        static void set_filepath(const char *filepath);
        ConfigHttp http;
        void getline(std::string &line);

        ConfigServer const* get_server(std::string const& port, std::string const& host) const;
        ConfigLocation const* get_location(std::string const& port, std::string const& host, std::string const& path) const;
        std::vector<std::string> get_location_paths(std::string const& port, std::string const& host) const;
        std::map<std::string, std::vector<std::string> > get_locations_contents(std::string const& port, std::string const& host,
                                                            std::string const& location) const;
        std::map<std::string, std::vector<std::string> > get_locations_properties(std::string const& port, std::string const& host,
                                                              std::string const& filepath) const;


    private:
        Config();
        static Config *_singleton;
        static std::string _filepath;
        std::ifstream _fin;
        static std::map<std::pair<std::string, std::string>, ConfigServer const*> _servers_cache;
        static std::map<std::pair<std::string, std::string>, std::vector<std::string> > _locations_cache;
        static std::map<std::pair<std::pair<std::string, std::string>, std::string>, std::map<std::string, std::vector<std::string> > > _locations_content_cache;
        static std::map<std::pair<std::pair<std::string, std::string>, std::string>, std::map<std::string, std::vector<std::string> > > _locations_properties_cache;

};
#endif
