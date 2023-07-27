#ifndef CONFIG_HTTP_HPP
#define CONFIG_HTTP_HPP

#include "config_server.hpp"

class ConfigHttp
{
    public:
        ConfigHttp();
        ~ConfigHttp();
        int client_max_body_size;
        std::vector<ConfigServer*> servers;
        void parse();
    private:

};

#endif
