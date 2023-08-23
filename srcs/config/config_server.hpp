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
        ~ConfigServer();
        ConfigLocation const *location(size_t i) const;
        size_t get_location_size() const;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigLocation*> const &vec);
        void check();
        std::string const &server_name() const;
        Port const & listen() const;
        bool is_default_server() const;
    private:

        std::vector<ConfigLocation*> locations;
        void set_listen(std::vector<std::string> &vec);
        void set_server_name(std::vector<std::string> &vec);
        Port listen_;
        std::string server_name_;
        bool is_default_server_;

};

#endif
