#ifndef CONFIG_LIMIT_HPP
#define CONFIG_LIMIT_HPP

#include "config_last_object.hpp"
#include "config_object.hpp"
#include "method.hpp"

class ConfigLimit : public ConfigObject<ConfigLastObject*>
{
    public:
        ConfigLimit();
        ~ConfigLimit();
        //std::map<std::string, std::vector<std::string> > properties;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        //void assign_properties();
    private:
        std::vector<std::string> out_properties;
        std::vector<std::string> deny;
        std::vector<std::string> allow;
        std::vector<Method> allow_method;
        void push_all(std::vector<ConfigLastObject*> const &vec);

};

#endif
