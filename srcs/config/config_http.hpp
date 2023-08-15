#ifndef CONFIG_HTTP_HPP
#define CONFIG_HTTP_HPP


#include <string>
#include "config_server.hpp"
#include "config_object.hpp"

class ConfigHttp : public ConfigObject<ConfigServer*>
{
    public:
        ConfigHttp();
        ConfigHttp(std::string &str);
        ~ConfigHttp();
        int client_max_body_size;
        std::vector<ConfigServer*> servers;
        std::map<std::string, std::string> map;
        void parse();
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigServer*> const &vec);
    private:

};

#endif
