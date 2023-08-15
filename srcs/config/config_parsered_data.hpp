#ifndef CONFIG_PARSERED_DATA_HPP
#define CONFIG_PARSERED_DATA_HPP

#include <vector>
#include <string>

class ConfigParseredData
{
    public:
        ConfigParseredData();
        ~ConfigParseredData();
        //void assign_next_target_properties(std::vector<std::string> &properties);
        //void set_next_string(std::vector<std::string> &vec);
        void push_back_next_string(std::string const &str);
        void push_back_next_properties(std::vector<std::string> &vec);
        std::string raw_data(size_t i);
        std::vector<std::string> &properties(size_t i);
        size_t size();
    private:
        std::vector<std::string> next_raw_data;
        std::vector<std::vector<std::string> > next_target_properties;
        std::string other_strings;
};
#endif
