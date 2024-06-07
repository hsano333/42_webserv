#ifndef CONFIG_LOADER
#define CONFIG_LOADER
#include <string>

class ConfigLoader
{
    public:
        ConfigLoader();
        ~ConfigLoader();
        void load_file(std::string& filepath);
        std::string loaded_data;
        void add_properties(std::vector<std::string> properties);
        void check_properties();
    private:

};
#endif
