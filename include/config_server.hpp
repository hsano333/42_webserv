#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include "config_location.hpp"
#include <vector>

class ConfigServer
{
    public:
        size_t size();
        std::string listen;
        std::string server_name;
        bool is_default_server;
        std::vector<ConfigLocation*> locations;
    private:

};

#endif
