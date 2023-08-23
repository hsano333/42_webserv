#ifndef CONFIG_LOCATION_HPP
#define CONFIG_LOCATION_HPP

#include <vector>
#include <map>
#include <string>
#include "config_last_object.hpp"
#include "config_object.hpp"
#include "config_limit.hpp"
#include "status_code.hpp"

class ConfigLocation : public ConfigObject<ConfigLimit*>
{
    public:
        ConfigLocation();
        ~ConfigLocation();
        ConfigLimit const *limit(size_t i) const;
        size_t get_limit_size() const;
        std::vector<std::string> const & pathes() const;
        std::string const & root() const;
        std::string const & cgi_pass() const;
        bool autoindex() const;
        std::vector<std::string> const &indexes() const;
        std::map<StatusCode, std::string> const &error_pages() const;

        //std::vector<std::string> urls;
        //std::map<std::string, std::vector<std::string> > properties;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigLimit*> const &vec);
        void check();

        //std::vector<std::string> out_properties;
        //std::string location;
        //std::vector<std::string> pathes;
        //std::map<StatusCode, std::string> error_pages;
        //std::vector<std::string> indexes;

    private:

        std::vector<ConfigLimit*> limits;
        std::vector<std::string> pathes_;
        std::string root_;
        std::string cgi_pass_;
        bool autoindex_;
        std::vector<std::string> indexes_;
        std::map<StatusCode, std::string> error_pages_;

        void set_root(std::vector<std::string> &vec);
        void set_cgi_pass(std::vector<std::string> &vec);
        void set_autoindex(std::vector<std::string> &vec);
        void set_index(std::vector<std::string> &vec);
        void set_error_page(std::vector<std::string> &vec);

};

#endif
