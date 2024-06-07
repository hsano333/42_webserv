#ifndef CONFIG_LIMIT_HPP
#define CONFIG_LIMIT_HPP

#include "config_last_object.hpp"
#include "config_object.hpp"
#include "config_cgi.hpp"
#include "method.hpp"
#include "cidr.hpp"

class ConfigLimit : public ConfigObject<ConfigCgi*>
{
    public:
        ConfigLimit();
        ~ConfigLimit();
        const ConfigCgi *cgi() const;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigCgi*> const &vec);
        void check();

        std::string allowed_method_str() const;
        std::vector<Method> const & allowed_method() const;
        std::vector<std::pair<CIDR, bool> > const &allowed_cidr() const;
    private:
        ConfigCgi* cgi_;
        std::vector<Method> allowed_method_;
        std::vector<std::pair<CIDR, bool> > allowd_cidr;

        void set_cidr(std::vector<std::string> &vec, bool flag);


};

#endif
