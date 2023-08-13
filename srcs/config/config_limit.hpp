#ifndef CONFIG_LIMIT_HPP
#define CONFIG_LIMIT_HPP

#include "config_last_object.hpp"
#include "config_object.hpp"

class ConfigLimit : public ConfigObject<ConfigLastObject>
{
    public:
        ConfigLimit();
        ~ConfigLimit();
        //std::map<std::string, std::vector<std::string> > properties;
        void add_properties(std::string &key, std::string &value);
        ConfigLastObject get_next_object();
    private:

};

#endif
