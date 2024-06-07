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
        const ConfigLimit *limit() const;
        size_t get_limit_size() const;
        std::vector<std::string> const & pathes() const;
        std::string const & root() const;
        bool autoindex() const;
        bool index() const;
        std::string const &index_file() const;
        bool is_redirect() const;
        const std::pair<StatusCode, std::string> &redirect() const;
        std::map<StatusCode, std::string> const &error_pages() const;
        StatusCode const &error_replaced_code() const;
        std::string get_error_file_path(StatusCode &code) const;
        bool auth_basic() const;
        std::string const &auth_basic_path() const;
        void assign_properties(std::vector<std::vector<std::string> > &properties);
        void assign_out_properties(std::vector<std::string> &properties);
        void push_all(std::vector<ConfigLimit*> const &vec);
        bool is_allowed_method(Method method) const;
        void check();

    private:

        void set_root(std::vector<std::string> &vec);
        void set_autoindex(std::vector<std::string> &vec);
        void set_index(std::vector<std::string> &vec);
        void set_return(std::vector<std::string> &vec);
        void set_error_page(std::vector<std::string> &vec);
        void set_auth_basic(std::vector<std::string> &vec);
        void set_auth_basic_user_file(std::vector<std::string> &vec);

        std::vector<std::string> pathes_;
        std::string root_;
        ConfigLimit* limit_;
        bool autoindex_;
        bool index_;
        std::string index_file_;
        std::map<StatusCode, std::string> error_pages_;
        std::map<std::string, std::string> cgi_;
        bool is_redirect_;
        std::pair<StatusCode, std::string> redirect_;
        bool auth_basic_;
        std::string auth_basic_path_;
        StatusCode error_replaced_code_;

};

#endif
