#ifndef CONFIG_CGI_HPP
#define CONFIG_CGI_HPP

#include "config_last_object.hpp"
#include "config_object.hpp"
#include "method.hpp"
#include <vector>
#include <string>
#include <map>
#include <sys/stat.h>


class ConfigCgi : public ConfigObject<ConfigLastObject*>
{
    public:
        ConfigCgi();
        ~ConfigCgi();
        std::map<std::string, std::string> &get_extensions();
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigLastObject*> const &vec);
        void check();

        std::vector<Method> const & allowed_method() const;
    private:
        std::map<std::string, std::string> extensions;
        //std::vector<Method> allowed_method_;

        //void set_cidr(std::vector<std::string> &vec, bool flag);


};

#endif
