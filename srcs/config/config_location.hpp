#ifndef CONFIG_LOCATION_HPP
#define CONFIG_LOCATION_HPP

#include <vector>
#include <map>
#include <string>
#include "config_last_object.hpp"
#include "config_object.hpp"
#include "config_limit.hpp"

class ConfigLocation : public ConfigObject<ConfigLimit>
{
    public:
        ConfigLocation();
        ConfigLocation(std::string &str);
        ~ConfigLocation();
        std::vector<std::string> urls;
        std::vector<ConfigLimit*> limits;
        std::map<std::string, std::vector<std::string> > properties;
        void add_properties(std::string &key, std::string &value);
        ConfigLimit get_next_object();
    private:


};

#endif
