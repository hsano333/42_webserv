#ifndef CONFIG_SERVER_HPP
#define CONFIG_SERVER_HPP

#include <string>
#include "config_location.hpp"
#include "config_object.hpp"
#include "port.hpp"
#include <vector>

class ConfigServer : public ConfigObject<ConfigLocation*>
{
    public:
        ConfigServer();
        ConfigServer(std::string &str);
        ~ConfigServer();
        size_t size();
        Port listen;
        std::string server_name;
        bool is_default_server;
        std::vector<ConfigLocation*> locations;
        std::map<std::string, std::string> map;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigLocation*> const &vec);
        //void assign_properties();
    private:
        void set_listen(std::vector<std::string> &vec);
        void set_server_name(std::vector<std::string> &vec);
        //std::string listen;
        //std::string server_name;

};

#endif
