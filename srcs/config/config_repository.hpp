#ifndef CONFIG_REPOSITORY_HPP
#define CONFIG_REPOSITORY_HPP
#include "file.hpp"
#include <string>

class ConfigRepository
{
    public:
        ConfigRepository(File file);
        ~ConfigRepository();
        //void load_file(std::string &filepath);
        void set_data(std::string &str);
    private:
        //File cfg_file;
        std::string config_raw_data;
};

#endif
