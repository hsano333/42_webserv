#ifndef CONFIG_HTTP_HPP
#define CONFIG_HTTP_HPP


#include <string>
#include "config_server.hpp"
#include "config_object.hpp"

class ConfigHttp : public ConfigObject<ConfigServer*>
{
    public:
        ConfigHttp();
        ~ConfigHttp();
        ConfigServer const *server(size_t i) const;
        size_t get_server_size() const;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigServer*> const &vec);
        void check();
    private:
        std::vector<ConfigServer*> servers;

};

#endif
