#ifndef CONFIG_LIMIT_HPP
#define CONFIG_LIMIT_HPP

#include "config_last_object.hpp"
#include "config_object.hpp"
#include "method.hpp"
#include "cidr.hpp"

class ConfigLimit : public ConfigObject<ConfigLastObject*>
{
    public:
        ConfigLimit();
        ~ConfigLimit();
        //std::map<std::string, std::vector<std::string> > properties;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void check();

        std::vector<Method> const & allowed_method() const;
        std::vector<std::pair<CIDR, bool> > const &allowed_cidr() const;
        //void assign_properties();
    private:
        //std::vector<std::string> deny;
        //std::vector<std::string> allow;
        std::vector<Method> allowed_method_;
        std::vector<std::pair<CIDR, bool> > allowd_cidr;
        void push_all(std::vector<ConfigLastObject*> const &vec);

        void set_cidr(std::vector<std::string> &vec, bool flag);


};

#endif
