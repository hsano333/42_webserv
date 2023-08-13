#ifndef CONFIG_HTTP_HPP
#define CONFIG_HTTP_HPP


#include <string>
#include "config_server.hpp"
#include "config_object.hpp"

class ConfigHttp : public ConfigObject<ConfigServer>
{
    public:
        ConfigHttp();
        ConfigHttp(std::string &str);
        ~ConfigHttp();
        int client_max_body_size;
        std::vector<ConfigServer*> servers;
        std::map<std::string, std::string> map;
        void parse();
        void add_properties(std::string &key, std::string &value);
        ConfigServer get_next_object();
    private:

};

#endif
