#ifndef CONFIG_RAW_LOADER
#define CONFIG_RAW_LOADER

#include "iconfig_raw_getter.hpp"
#include "normal_file.hpp"

class ConfigRawLoader : public IConfigRawGetter<std::string>
{
    public:
        ConfigRawLoader(NormalFile *file);
        ~ConfigRawLoader();
        std::string get_raw_data();
    private:
        NormalFile* file;
};

#endif

