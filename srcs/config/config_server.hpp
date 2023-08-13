#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include <string>
#include "config_location.hpp"
#include "config_object.hpp"
#include <vector>

class ConfigServer : public ConfigObject<ConfigLocation>
{
    public:
        ConfigServer();
        ConfigServer(std::string &str);
        ~ConfigServer();
        size_t size();
        std::string listen;
        std::string server_name;
        bool is_default_server;
        std::vector<ConfigLocation*> locations;
        std::map<std::string, std::string> map;
        void add_properties(std::string &key, std::string &value);
        ConfigLocation get_next_object();
    private:

};

#endif
