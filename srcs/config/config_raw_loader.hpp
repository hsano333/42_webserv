#ifndef CONFIG_RAW_LOADER
#define CONFIG_RAW_LOADER

#include "iconfig_raw_getter.hpp"
//#include "webserv_file.hpp"
#include "normal_file.hpp"

class ConfigRawLoader : public IConfigRawGetter<std::string>
{
    public:
        ConfigRawLoader(NormalFile *file);
        ~ConfigRawLoader();
        std::string get_raw_data();
    private:
        //std::basic_string<T, std::char_traits<T>, std::allocator<T> > get_raw_data();
        NormalFile* file;
};

/*
template <typename T>
std::string ConfigRawLoader<T>::get_raw_data()
{
    file.open_file();
    T tmp = "test";
    return tmp;
    //return file.read();
}
*/


#endif

